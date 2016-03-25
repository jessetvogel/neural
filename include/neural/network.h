#ifndef NETWORK_H
#define NETWORK_H

#include "layer.h"

class Network
{
public:
    static double alpha;
    
    int amount_of_layers;
    Layer** layers;
    
    double* feed(double*);
    void train(double*, double*);
    
    Network(int, int*);
    ~Network();
    
    int save(const char[]);
    Network(const char[]);
};

#endif
