#ifndef LAYER_H
#define LAYER_H

#include <cstddef>
#include <iostream>

class Layer
{
public:
    int amount_of_nodes;
    double* weights;
    double* values;
    double* deltas;
    Layer* previous_layer;
    
    void evaluate();
    void backpropagate();
    
    ~Layer();
};

#endif
