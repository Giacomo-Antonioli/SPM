// we include farm, pipeline and allocator hpp files
// as we use only this three features of FastFlow
#include <ff/node.hpp>
#include <ff/pipeline.hpp>
#include <ff/allocator.hpp>
#include <iostream>

// we use the FastFlow namespace
using namespace ff;

// we use the fastflow mem allocator
// should be initialized (main) before actually using it
static ff_allocator allocator;

// we implement a pipeline application here:
//
//  Pipeline
//
//    +------+    +------+  +------+  +------+
//    |Stage1| -> |Stage2|->|Stage3|->|Stage4|
//    +------+    +------+  +------+  +------+
//
//  Stage1 -> generate an integer stream
//  StageN -> prints the input stream
//  StageK -> increases integers
//

// definition of the generic inc stage
// subclass ff_node (seq code wrapper)

class IntIncStage:public ff_node {

    // method called on initialization (optional)
    int svc_init() {
        std::cout << "IntIncStage " << ff_node::get_my_id()
                  << " inited !" << std::endl;
    }

    // method wrapping "task computation code"
    // for each task on the input stream, this method is called
    // and it returns the data to be placed onto the output stream
    // in and out data are pointers, as usual
    void * svc (void * task) {
        int * i = (int *) task;
        std::cout << "Stage " << ff_node::get_my_id()
                  << " got task " << *i ;
        (*i)++;
        std::cout << " computed " << *i << std::endl;
        return task;
    }

    // this  is called before termination
    // (optional)
    void svc_end() {
        std::cout << "Stage " << ff_node::get_my_id()
                  << " terminating " << std::endl;
    }
};

// last stage: print in stream contents
// it's sequential => subclass ff_node
class DrainStage:public ff_node {

    // print initialization message
    int svc_init() {
        std::cout << "DrainStage " << ff_node::get_my_id()
                  << " inited !" << std::endl;
    }

    // stage body: actual wrapping the sequential code
    void * svc (void * task) {
        int * i = (int *) task;
        std::cout << "Stage " << ff_node::get_my_id()
                  << " got result " << *i << std::endl;
        allocator.free(task);
        return task;
    }
};

// stage generating the stream
// seq => subclass ff_node
// to output data items on the output stream uses ff_send_out()
// to terminate the stream, outputs a FF_EOS
// to terminate, returns a NULL
class GenerateStream:public ff_node {

private:
    int ntasks;

public:
    // constructor: used to pass the stream "size"
    GenerateStream(int ntasks):ntasks(ntasks) { }

    int svc_init() {
        std::cout << "GenerateStream(" << ntasks << ") "
                  << ff_node::get_my_id() << " inited !" << std::endl;
    }

    // Stage body: output a stream with ntasks, ntasks-1, ntasks-2, ...
    // down to 0
    // then output the FF_EOS (end of stream)
    // and terminate (return NULL)

    void * svc(void * task) {
        while(ntasks != 0) {
            int * tt = (int *) allocator.malloc(sizeof(int));
            *tt = ntasks;
            ff::ff_node::ff_send_out((void *) tt);
            ntasks--;
            std::cout << "GenerateStream(" << ntasks << ")" << std::endl;
        }
        ff_send_out(EOS);
        return NULL;
    }

};


int main(int argc, char * argv[]) {
    int tasks = 10;  // dummy, should be taken from the input line

    // allocator must be initialized before using it
    allocator.init();

    // declare a pipeline object
    ff_pipeline pipe_a;
    // add stages in order:
    //    the first stage added is the first pipeline stage
    //    the i-th stage added is the i-th pipeline stage
    pipe_a.add_stage(new GenerateStream(tasks));
    pipe_a.add_stage(new IntIncStage);
    pipe_a.add_stage(new IntIncStage);
    pipe_a.add_stage(new DrainStage);


    // now we ca run the application:
    std::cout << "Starting application ..." << std::endl;
    pipe_a.run();
    std::cout << "Application started" << std::endl;

    // here more (unrelated) work can be performed ...
    // when results are needed we should wait for application termination
    pipe_a.wait();
    // alternatively, if we have nothing else to do, we can issue a single
    // call such as:     pipe_a.run_and_wait_end();
    // in this case the pipeline is started and its termination awaited
    // synchronously
    std::cout << "Application terminated" << std::endl;
    return(0);
}