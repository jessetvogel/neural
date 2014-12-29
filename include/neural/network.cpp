#include "input.h"
#include "node.h"
#include "layer.h"
#include "network.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

double Network::trainingConstant = 1.0;

Network::Network(int _size, int* _layerSize)
{
    // Normal constructor
    init(_size, _layerSize);
}

Network::Network(char path[])
{
    // Open file
    std::ifstream in(path);
    
    // Read the size of the network / the amount of layers
    in.read((char*) &size, sizeof(int));
    
    // Read the sizes of each layer / the amount of nodes per layer
    int* layerSize = (int*) malloc(size * sizeof(int));
    in.read((char*) layerSize, size * sizeof(int));
    
    // Now that the size of the network and the sizes of the layers are known, create / initialize the network
    init(size, layerSize);
    
    // Free the allocated space
    free(layerSize);
    
    // For each layer, read the weights
    for(int i = 1;i < size;i ++)
    {
        int n = layers[i]->size * (layers[i - 1]->size + 1) * sizeof(T_WEIGHT);
        in.read((char*) (layers[i]->weights), n);
    }

    // Close the stream
    in.close();
}

void Network::init(int _size, int* _layerSize)
{
    // Seed the random generator
    srand(time(NULL));
    
    // Create an array of Layer's of same length as nodes_per_layer
    size = _size;
    layers = (Layer**) malloc(size * sizeof(Layer*));
    
    // First create the input layer
    layers[0] = (Layer*) malloc(sizeof(Layer));
    new(layers[0]) Layer(LAYER_TYPE_INPUT, _layerSize[0]);
    
    for(int i = 1;i < _size;i ++)
    {
        // Create the hidden and output layer(s) and connect each with its previous layer
        layers[i] = (Layer*) malloc(sizeof(Layer));
        
        // All hidden types, except for the last one
        int type = i < _size - 1 ? LAYER_TYPE_HIDDEN : LAYER_TYPE_OUTPUT;
        
        new(layers[i]) Layer(type, _layerSize[i]);
        layers[i]->setPreviousLayer(layers[i-1]);
    }
}

T_NODE_VALUE* Network::input(T_NODE_VALUE* _input)
{
    // Set the input values of the inputs in the first layer
    layers[0]->input(_input);
    
    // Return an array of the outputs of the last layer
    return layers[size - 1]->output();
}

T_NODE_VALUE* Network::input(Input* _input)
{
    // Use the Input _input as input for the earlier defined function
    return input(_input->input);
}

void Network::train(T_NODE_VALUE* _input, T_NODE_VALUE* _output)
{
    // Input the input and evaluate everything
    input(_input);

    // Calculate the error for each node in the output layer
    Layer* outputLayer = layers[size - 1];
    
    for(int i = 0;i < outputLayer->size;i ++)
    {
        Node* node = outputLayer->nodes[i];
        node->error = node->output * (1 - node->output) * (_output[i] - node->output);
    }
    
    // Start Back Propagation chain
    outputLayer->backPropagate();
}

int Network::save(char path[])
{
    std::ofstream out(path);
    
    // If something went wrong
    if(out < 0) return 0;
    
    // Write the size of the network / amount of layers
    out.write((char*) &size, sizeof(int));
    
    // Write the size of each layer
    for(int i = 0;i < size;i ++) out.write((char*) &(layers[i]->size), sizeof(int));

    // Write each array of weights to the file
    for(int i = 1;i < size;i ++)
    {
        int n = layers[i]->size * (layers[i - 1]->size + 1) * sizeof(T_WEIGHT);
        out.write((char*) (layers[i]->weights), n);
    }
    
    out.close();
    
    return 1;
}

Network::~Network()
{
    // Destruct all the layers
    for(int i = 0;i < size;i ++)
    {
        layers[i]->~Layer();
        free(layers[i]);
    }

    // Free the malloc'ed space
    free(layers);
}

