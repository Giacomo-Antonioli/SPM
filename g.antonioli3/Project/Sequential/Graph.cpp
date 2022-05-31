//
// Created by giaco on 18/05/2022.
//

#include "Graph.h"
#include <queue>
#include <condition_variable>

template<typename QTYPE> //Create my own shared queue

class sharedQueue {

private:
    std::queue <QTYPE> q;       //Queue
    std::mutex m;               //Mutex to protect the queue
    std::condition_variable c;  //Condition variable to notify queue usage

public:
    QTYPE get_element() {
        std::unique_lock <std::mutex> l(m);
        while (q.empty())
            c.wait(l);
        QTYPE task = q.front();
        q.pop();
        return task;
    }

    void add_element(QTYPE element) {
        std::unique_lock <std::mutex> l(m);
        q.push(element);
        c.notify_one();
    }

};
sharedQueue<float> test[10];
void Graph::addNode(Node *node_to_be_added) {
    Graph::node_list.push_back( node_to_be_added);
}

void Graph::generateStruct() {
    std::cout << "genstrcut" << std::endl;
    for(int i=0;i< this->node_list.size();i++)
    { std::cout << "yo" << std::endl;

    this->output_sum+=this->node_list[i]->getOutputArity();}
    std::cout<<"TOTAL OUTPUT ARIETY: "<< this->output_sum;


}
void Graph::compute() {
    // int x= 100;
    std::cout<<"CANEBAU"<<std::endl;
    this->generateStruct();
    std::cout << "Compiling" << std::endl;

    for(int i=0;i< this->node_list.size();i++)
    {
        auto  compute_function=this->node_list[i]->getToCompute();
        // add retrive
        if(this->node_list[i]!=0)
            this->node_list[i]->fetch_inputs();
        compute_function();
        // add push
    }
}


