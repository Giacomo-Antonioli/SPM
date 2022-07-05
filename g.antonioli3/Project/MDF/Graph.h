//
// Created by giaco on 18/05/2022.
//
#include "Node.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

#ifndef SPM_GRAPH_H
#define SPM_GRAPH_H


class Graph {
public:


private:
    std::vector<Node *> node_list;
    int output_sum = -1;


public:
    void addNode(Node *node_to_be_added);

    void generateStruct();

    void compute();

    float result;

    void parallel_compute(int i);

};


#endif //SPM_GRAPH_H
