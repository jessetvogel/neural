#include "input.h"
#include "node.h"
#include "layer.h"
#include "network.h"

#include <cstdlib>
#include <iostream>

Layer::Layer(int _type, int _size)
{
    // Store this data
    type = _type;
    size = _size;
    
    // Create an array of new nodes
    nodes = (Node**) malloc(size * sizeof(Node*));
    for(int i = 0;i < size;i ++)
    {
        nodes[i] = (Node*) malloc(sizeof(Node));
        new(nodes[i]) Node();
    }
}

T_WEIGHT Layer::getWeight(int _from, int _to)
{
    return weights[_from * size + _to];
}

void Layer::setWeight(int _from, int _to, T_WEIGHT _value)
{
    weights[_from * size + _to] = _value;
}

void Layer::setPreviousLayer(Layer* _previousLayer)
{
    // Connect the layers
    previousLayer = _previousLayer;
    previousLayer->nextLayer = this;
    
    // Create an array of weights
    weights = (T_WEIGHT*) malloc(size * (previousLayer->size + 1) * sizeof(T_WEIGHT));
    
    // Fill the array with random weights from -1 to 1
    for(int i = 0;i < previousLayer->size;i ++)
    {
        for(int j = 0;j < size;j ++)
        {
//            setWeight(i, j, (T_WEIGHT) rand() * 2 / RAND_MAX - 1.0);
            setWeight(i, j, 0);
        }
    }
}

void Layer::input(T_NODE_VALUE* _input)
{
    if(type != LAYER_TYPE_INPUT)
    {
        // TODO possibly return an error (?)
        return;
    }
    
    // Set the input node values with the inputs
    for(int i = 0;i < size;i ++)
    {
        nodes[i]->output = _input[i];
    }
    
    // Each of the nodes from this layers has their output set. Now the next layer can be evaluated
    nextLayer->evaluate();
}

void Layer::evaluate()
{
    // Evaluate for each node in the layer
    for(int i = 0;i < size;i ++)
    {
        // Get the sum of input * weight for each of the nodes from the previous layer
        double sum = getWeight(previousLayer->size, i); // Start with the bias, which equals 1 * weight[BIAS]
        
        for(int j = 0;j < previousLayer->size;j ++)
        {
            sum += previousLayer->nodes[j]->output * getWeight(j, i);
        }
    
        // Let the sum go through the activation function
        nodes[i]->output = Node::activate(sum);
    }
    
    // Stop the chain of this is the last layer of the network
    if(type == LAYER_TYPE_OUTPUT) return;
    
    // Each of the nodes from this layers has their output set. Now the next layer can be evaluated.
    nextLayer->evaluate();
}

T_NODE_VALUE* Layer::output()
{
    if(type != LAYER_TYPE_OUTPUT)
    {
        // TODO log something, or give/throw an error?
        return NULL;
    }
    
    // Create an array for the outputs
    T_NODE_VALUE* output = (T_NODE_VALUE*) malloc(size * sizeof(T_NODE_VALUE));
    
    // Get each individual output
    for(int i = 0;i < size;i ++)
    {
        output[i] = nodes[i]->output;
    }
    
    return output;
}

void Layer::backPropagate()
{
    if(type == LAYER_TYPE_INPUT)
    {
        // Stop the chain
        return;
    }
    
    // First calculate errors in the previous layer, since we need the old weights (!)
    for(int i = 0;i < previousLayer->size;i ++)
    {
        // Calculate the weighted sum of errors
        double errorSum = 0;
        for(int j = 0;j < size;j ++)
        {
            errorSum += nodes[j]->error * getWeight(i, j);
        }
        
        // Set the error
        Node* node = previousLayer->nodes[i];
        node->error = node->output * (1 - node->output) * errorSum;
    }
    
    // For each node in this layer
    for(int j = 0;j < size;j ++)
    {
        // Update the weight to that node from every one in the previous layer
        for(int i = 0;i < previousLayer->size;i ++)
        {
            // Update according to: W_{ij}^{+} = W_{ij} + c * error_{j} * output_{i}
            setWeight(i, j, getWeight(i, j) + Network::trainingConstant * nodes[j]->error * previousLayer->nodes[i]->output);
        }
        
        // Update the weight of the bias
        setWeight(previousLayer->size, j, getWeight(previousLayer->size, j) + Network::trainingConstant * nodes[j]->error);
    }
    
    // Continue the backpropagation
    previousLayer->backPropagate();
}

Layer::~Layer()
{
    // Destruct all the nodes
    for(int i = 0;i < size;i ++)
    {
        nodes[i]->~Node();
        free(nodes[i]);
    }
    
    // Free all the malloc'ed space
    free(nodes);
    if(type != LAYER_TYPE_INPUT) free(weights);
}
