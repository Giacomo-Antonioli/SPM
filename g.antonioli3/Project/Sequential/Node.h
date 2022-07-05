//
// Created by giaco on 18/05/2022.
//
#include <stdlib.h>
#include <vector>
#include <functional>
#ifndef SPM_NODE_H
#define SPM_NODE_H
#include "shared_queue.cpp"

class Node {
private:
    int id;
    int input_arity;
    int output_arity;
    std::vector<Node*> dependecy_node_list;
    std::function<void(void)> to_compute;


public:
    std::vector<int> inputs;
    std::vector<int> outputs;
    int inner_counter=0;
    int inner_counter_output=0;
    std::vector<sharedQueue<float> *> output_vars;
    std::vector<sharedQueue<float> *> input_vars;
    Node(const int id = 0, const int input_arity = 1, const int output_arity = 1);
    void addDep(Node* addedNode);
    void addCompute(std::function<void(void)> to_be_added);

    int  getElement_inputs(int index){
        return Node::inputs[index];
    }
    int  getElement_outputs(int index){
        return Node::outputs[index];
    }
    int getId() const;

    void setId(int id);

    int getInputArity() const;

    void setInputArity(int inputArity);

    int getOutputArity() const;

    void setOutputArity(int outputArity);

    const std::vector<Node *> &getDependecyNodeList() const;

    void setDependecyNodeList(const std::vector<Node *> &dependecyNodeList);

    const std::function<void(void)> &getToCompute() const;

    void setToCompute(const std::function<void(void)> &toCompute);

    int getX() const;

    void setX(int x);

    int getZb() const;

    void setZb(int zb);

    int getZc() const;

    void setZc(int zc);

    int getYb() const;

    void setYb(int yb);

    int getYc() const;

    void setYc(int yc);

    int getRes() const;

    void setRes(int res);

    const std::vector<int> &getInputs() const;

    void setInputs(const std::vector<int> &inputs);

    const std::vector<int> &getOutputs() const;

    void setOutputs(const std::vector<int> &outputs);

};


#endif //SPM_NODE_H
