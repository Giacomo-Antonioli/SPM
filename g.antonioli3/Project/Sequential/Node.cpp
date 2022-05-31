//
// Created by giaco on 18/05/2022.
//

#include "Node.h"

Node::Node(const int id, const int input_arity, const int output_arity){
    Node::id=id;
    Node::input_arity=input_arity;
    Node::output_arity=output_arity;
    Node::inputs.resize(input_arity);
    Node::outputs.resize(output_arity);

}
void Node::addDep(Node *addedNode) {
    Node::dependecy_node_list.push_back(addedNode);
}

void Node::addCompute(std::function<void(void)> to_be_added){
    Node::to_compute=to_be_added;
}

void Node::fetch_inputs() {

}


int Node::getId() const {
    return id;
}

void Node::setId(int id) {
    Node::id = id;
}

int Node::getInputArity() const {
    return input_arity;
}

void Node::setInputArity(int inputArity) {
    input_arity = inputArity;
}

int Node::getOutputArity() const {
    return output_arity;
}

void Node::setOutputArity(int outputArity) {
    output_arity = outputArity;
}

const std::vector<Node *> &Node::getDependecyNodeList() const {
    return dependecy_node_list;
}

void Node::setDependecyNodeList(const std::vector<Node *> &dependecyNodeList) {
    dependecy_node_list = dependecyNodeList;
}

const std::function<void(void)> &Node::getToCompute() const {
    return to_compute;
}

void Node::setToCompute(const std::function<void(void)> &toCompute) {
    to_compute = toCompute;
}


const std::vector<int> &Node::getInputs() const {
    return inputs;
}

void Node::setInputs(const std::vector<int> &inputs) {
    Node::inputs = inputs;
}

const std::vector<int> &Node::getOutputs() const {
    return outputs;
}

void Node::setOutputs(const std::vector<int> &outputs) {
    Node::outputs = outputs;
}

