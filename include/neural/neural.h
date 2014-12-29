//
//  neural.h
//
//
//  Created by Jesse on 16-11-14.
//
//

#ifndef _neural_h
#define _neural_h

#include "input.h"
#include "node.h"
#include "layer.h"
#include "network.h"

#include <iostream>

// Typedef
typedef double T_WEIGHT;
typedef double T_NODE_VALUE;

void showWeights(Network* network)
{
    for(int i = 1;i < network->size;i ++)
    {
        T_WEIGHT* w = network->layers[i]->weights;
        int n = network->layers[i]->size * (network->layers[i-1]->size + 1);
        
        for(int j = 0;j < n;j ++) std::cout << (w[j]) << " ";
        std::cout << "\n\n";
    }
}

#endif
