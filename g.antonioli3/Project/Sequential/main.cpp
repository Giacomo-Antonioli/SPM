//
// Created by giaco on 18/05/2022.
//
#include <stdlib.h>
#include <iostream>
#include "Node.h"
#include "Graph.h"
int main() {
    int x=4;
    int zb,zc;
    int yb,yc;
    int res;

    std::cout << "Compiling" << std::endl;

    Node A(1,1,2);
    Node B(2,1,1);
   // Node C(3,1,1);
   // Node D(4,2,1);


    A.addDep(&B); //A.addDep(&C);
   // B.addDep(&D); C.addDep(&D);
A.inputs[0]=10;
    A.addCompute([&yb=A.outputs[0],&x=A.inputs[0],&yc=A.outputs[1]]() { yb = x - 1; yc = x + 1; });
   //A.addCompute([&](){std::cout<<"CANEBAU"<<std::endl;});


    B.addCompute([&zb=B.outputs[0],&yb=B.inputs[0]]() { zb = yb*2; });
    /*  C.addCompute([&]() { zc = yc*3; });
     D.addCompute([&]() { res = zb+zc; });
 */
    Graph g;
    g.addNode(&A); //g.addNode(&B);
    //g.addNode(&C); g.addNode(&D);

    g.compute();
    /*
    Node a(1, 1, 1);
    Node b(2, 1, 1);
    Graph g;

    a.addDep(&b);
    a.addCompute([&]() {y = x + 1;});
    b.addCompute([&]() {z = y + 10;});

    g.addNode(&a);
    g.addNode(&b);
    g.compute();*/

    std::cout << "Result (x)" << std::endl;
    std::cout<<A.getElement_inputs(0)<<std::endl;
    std::cout << "Result (yc)" << std::endl;
    std::cout<<A.getElement_outputs(1)<<std::endl;

    return 0;
}