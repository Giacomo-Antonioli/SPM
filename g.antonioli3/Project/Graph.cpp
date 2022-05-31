//
// Created by giaco on 18/05/2022.
//

#include "Graph.h"
#include "utimer.cpp"
#include <thread>
#include <utility>
#include <future>

void Graph::addNode(Node *node_to_be_added) {
    Graph::node_list.push_back(node_to_be_added);
}

void Graph::generateStruct() {
    std::cout << "Generating Structure" << std::endl;
    for (int i = 0; i < this->node_list.size(); i++) {
        this->output_sum += this->node_list[i]->getOutputArity();
        // std::cout<<this->output_sum<<std::endl;
    }
    //std::cout<<"TOTAL OUTPUT ARIETY: "<< this->output_sum<<std::endl;




}

void Graph::parallel_compute(int i) {
    bool exit= false;
    while(1 && !exit) {
        {

            // utimer tims("internal_computation_time");
            for (int j = 0; j < this->node_list[i]->getInputArity(); j++) {
                this->node_list[i]->inputs[j] = this->node_list[i]->input_vars[j]->get_element();
                if( this->node_list[i]->inputs[j]==-100)
                    exit=true;
            }
            if(!exit) {
               // std::cout << "computing node: " << i + 1 << std::endl;
                auto compute_function = this->node_list[i]->getToCompute();

                compute_function();
            }
            //  std::cout << "function computed" << std::endl;

            for (int j = 0; j < this->node_list[i]->getOutputArity(); j++) {
                // std::cout << "output[" << j << "]:  " << this->node_list[i]->outputs[j] << std::endl;
                if(!exit)
                this->node_list[i]->output_vars[j]->add_element(this->node_list[i]->outputs[j]);
                else
                    this->node_list[i]->output_vars[j]->add_element(-100);


            }
            //    std::cout << "**************************************" << std::endl;


        }
    }
}

void Graph::compute() {
    // int x= 100;
    this->generateStruct();

    sharedQueue<float> test[this->output_sum];
    std::cout << "total connections to create: " << this->output_sum << std::endl;
    std::cout << "#################################################" << std::endl;
    int dep_counter = 0;
    {
        utimer tim("compilation time");

        for (int i = 0; i < this->node_list.size(); i++) {
            std::cout << "[ID]: " << this->node_list[i]->getId() << "\tInput Ariety: "
                      << this->node_list[i]->getInputArity() << "\t Output Ariety: "
                      << this->node_list[i]->getOutputArity()
                      << std::endl;
            if (dep_counter < this->output_sum) {
                for (int current_ariety = 0; current_ariety < this->node_list[i]->getOutputArity(); current_ariety++) {
                    auto current_dependecy_list = this->node_list[i]->getDependecyNodeList();
                    std::cout << "NODE with ID: " << current_dependecy_list[current_ariety]->getId()
                              << " depends from this node" << std::endl;
                    this->node_list[i]->output_vars[this->node_list[i]->inner_counter] = &test[dep_counter];
                    this->node_list[current_dependecy_list[current_ariety]->getId() -
                                    1]->input_vars[current_dependecy_list[current_ariety]->inner_counter_output] = &test[dep_counter];
                    this->node_list[i]->inner_counter += 1;
                    this->node_list[current_dependecy_list[current_ariety]->getId() - 1]->inner_counter_output += 1;
                    dep_counter += 1;
                }
            }
            std::cout << "#################################################" << std::endl;

        }//GENERATING SUPPORT STRUCT
    }


    std::cout << "computing" << std::endl;
    sharedQueue<float> first_elem;
    sharedQueue<float> last_elem;
    this->node_list[0]->input_vars[0] = &first_elem;
    this->node_list[this->node_list.size() - 1]->output_vars[0] = &last_elem;

    first_elem.add_element(7.0);
    // std::cout << "first elem in" << std::endl;
    std::cout << "SEQUENTIAL CODE" << std::endl;
    {
        utimer tims("seq_comp_time");

        for (int i = 0; i < this->node_list.size(); i++) { //Seq
           //std::cout << "Computing node " << this->node_list[i]->getId() << std::endl;
            for (int j = 0; j < this->node_list[i]->getInputArity(); j++) {
                this->node_list[i]->inputs[j] = this->node_list[i]->input_vars[j]->get_element();
            }
            auto compute_function = this->node_list[i]->getToCompute();

            compute_function();

            // std::cout << "function computed" << std::endl;

            for (int j = 0; j < this->node_list[i]->getOutputArity(); j++) {
                //   std::cout << "output[" << j << "]:  " << this->node_list[i]->outputs[j] << std::endl;
                this->node_list[i]->output_vars[j]->add_element(this->node_list[i]->outputs[j]);
            }
            //std::cout << "**************************************" << std::endl;

        }
    }

    for(int i=0;i<10;i++)
    first_elem.add_element(i);
    first_elem.add_element(-100);
    std::vector<std::thread> tids;
    std::cout << "PARALLEL CODE" << std::endl;
    {
        utimer tims("parallel_comp_time");
        for (int i = 0; i < this->node_list.size(); i++) {
            tids.push_back(std::thread([this, i] { this->parallel_compute(i); }));
            //   = new std::thread(&Graph::parallel_compute,this, i);
        }
        for (int i = 0; i < this->node_list.size(); i++) {
            tids[i].join();
            //   = new std::thread(&Graph::parallel_compute,this, i);
        }
/*        for (int i = 0; i < this->node_list.size(); i++) { //Seq
            std::cout << "Computing node " << this->node_list[i]->getId() << std::endl;
            for (int j = 0; j < this->node_list[i]->getInputArity(); j++) {
                this->node_list[i]->inputs[j] = this->node_list[i]->input_vars[j]->get_element();
            }
            auto compute_function = this->node_list[i]->getToCompute();

            compute_function();

            std::cout << "function computed" << std::endl;

            for (int j = 0; j < this->node_list[i]->getOutputArity(); j++) {
                std::cout << "output[" << j << "]:  " << this->node_list[i]->outputs[j] << std::endl;
                this->node_list[i]->output_vars[j]->add_element(this->node_list[i]->outputs[j]);
            }
            std::cout << "**************************************" << std::endl;

        }*/
    }
    this->result = last_elem.get_element();
    int count_res=0;
    while(this->result!=-100){
        std::cout<<"Res["<<count_res<<"]: "<<this->result<<std::endl;
        count_res++;
    this->result = last_elem.get_element();}
}

