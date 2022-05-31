//
// Created by giaco on 18/05/2022.
//
#include <stdlib.h>
#include <iostream>
#include "Node.h"
#include "Graph.h"
#include <thread>
#include <chrono>
#include "utimer.cpp"
int main() {
    int x=4;
    int zb,zc;
    int yb,yc;
    int res;

    std::cout << "Compiling" << std::endl;
/*
    Node A(1,1,3);
    Node B(2,1,1);
    Node C(3,1,2);
    Node D(4,2,1);
    Node E(5,3,1);

    A.addDep(&B);
    A.addDep(&C);
    A.addDep(&E);
    B.addDep(&D);
    C.addDep(&D);
    C.addDep(&E);
    D.addDep(&E);

    A.addCompute([&ab=A.outputs[0],&ac=A.outputs[1],&ae=A.outputs[2],&x=A.inputs[0]]() { ae = x + 3;ab = x + 1;ac = x + 2;std::this_thread::sleep_for (std::chrono::seconds(2));  });
    B.addCompute([&bd=B.outputs[0],&ba=B.inputs[0]]() { bd=ba+10;std::this_thread::sleep_for (std::chrono::seconds(1));  });
    C.addCompute([&cd=C.outputs[0],&ce=C.outputs[1],&ca=C.inputs[0]]() { cd=ca*3;ce=ca+50; std::this_thread::sleep_for (std::chrono::seconds(2)); });
    D.addCompute([&de=D.outputs[0],&bc=D.inputs[0],&cd=D.inputs[1]]() { de=bc+cd;std::this_thread::sleep_for (std::chrono::seconds(2));  });
    E.addCompute([&res=E.outputs[0],&ae=E.inputs[0],&ce=E.inputs[1],&de=E.inputs[2]]() { res=ae*ce+de; std::this_thread::sleep_for (std::chrono::seconds(2)); });
    Graph g;
    g.addNode(&A);
    g.addNode(&B); g.addNode(&C); g.addNode(&D);
    g.addNode(&E);

g.compute();
    std::cout << "Result (x)" << std::endl;
    std::cout<<g.result<<std::endl;

*/
    Node A(1,1,5);//4
    Node B(2,1,2);//2
    Node C(3,1,1);
    Node D(4,1,1);
    Node E(5,1,1);
    Node F(6,3,1);
    Node G(7,1,1);
    Node H(8,1,1);
    Node I(9,4,1);


    A.addDep(&B);// A.addDep(&C);
    //B.addDep(&D); C.addDep(&D);
    //A.addDep(&B);
    A.addDep(&C);
    A.addDep(&D);
    A.addDep(&E);
    A.addDep(&H);
    B.addDep(&G);
    B.addDep(&I);
    C.addDep(&F);
    D.addDep(&F);
    E.addDep(&F);
    F.addDep(&I);
    G.addDep(&I);
    H.addDep(&I);

    // A.addCompute([&yb=A.outputs[0],&x=A.inputs[0],&yc=A.outputs[1]]() { yb = x - 1; yc = x + 1; });
    A.addCompute([&ab=A.outputs[0],&ac=A.outputs[1],&ad=A.outputs[2],&ae=A.outputs[3],&ai=A.outputs[4],&x=A.inputs[0]]() { ae = x + 1;ab = x + 1;ac = x + 1; ad = x + 1;ai=0; std::this_thread::sleep_for (std::chrono::seconds(2)); });
    B.addCompute([&bg=B.outputs[0],&bh=B.outputs[1],&ba=B.inputs[0]]() { bg=ba+1; bh=ba+1; std::this_thread::sleep_for (std::chrono::seconds(2));   });
    C.addCompute([&cf=C.outputs[0],&ca=C.inputs[0]]() { cf=ca+1; std::this_thread::sleep_for (std::chrono::seconds(2));  });
    D.addCompute([&df=D.outputs[0],&da=D.inputs[0]]() { df=da+1; std::this_thread::sleep_for (std::chrono::seconds(2));  });
    E.addCompute([&ef=E.outputs[0],&ea=E.inputs[0]]() { ef=ea+1; std::this_thread::sleep_for (std::chrono::seconds(2));  });
    F.addCompute([&fh=F.outputs[0],&fc=F.inputs[0],&fd=F.inputs[1],&fe=F.inputs[2]]() { fh=fc+fd+fe+1; std::this_thread::sleep_for (std::chrono::seconds(2));  });
    G.addCompute([&gh=G.outputs[0],&gb=G.inputs[0]]() { gh=gb+1; std::this_thread::sleep_for (std::chrono::seconds(2));  });
    H.addCompute([&ih=H.outputs[0],&ia=H.inputs[0]]() { ih=ia; std::this_thread::sleep_for (std::chrono::seconds(4));  });
    I.addCompute([&res=I.outputs[0],&hb=I.inputs[0],&hg=I.inputs[1],&hf=I.inputs[2],&hi=I.inputs[3]]() { res=hb+hg+hf+hi+1; std::this_thread::sleep_for (std::chrono::seconds(2));  });

 //  A.addCompute([&](){std::cout<<"CANEBAU"<<std::endl;});


    Graph g;
    g.addNode(&A);
    g.addNode(&B); g.addNode(&C); g.addNode(&D);
    g.addNode(&E);g.addNode(&F);g.addNode(&G);
    g.addNode(&H); g.addNode(&I);

  g.compute();
    std::cout << "Result (x)" << std::endl;
    std::cout<<g.result<<std::endl;




    return 0;
}