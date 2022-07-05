//
// Created by giaco on 30/06/2022.
//

#include <stdlib.h>
#include <iostream>
#include "Node.h"
#include "Graph.h"
int main() {
    int n=10;
    for(int i =0;i<n;i++)
    {
        int c=i;
        std::cout<<"pointer val= "<<&c<<std::endl;
    }

    return 0;
}

