#include <fstream>

#include "network.h"
#include "functions.h"

double Network::alpha = 0.1;

Network::Network(int amount_of_layers, int* nodes_per_layer)
{
    // Store amount of layers
    this->amount_of_layers = amount_of_layers;
    
    // Create an array of layers
    layers = new Layer*[amount_of_layers];
    
    for(int l = 0;l < amount_of_layers;l ++)
    {
        Layer* layer = layers[l] = new Layer();
        
        // Set amount of nodes
        layer->amount_of_nodes = nodes_per_layer[l];
        
        // Create an array of values and one of deltas
        layer->values = new double[nodes_per_layer[l]];
        layer->deltas = new double[nodes_per_layer[l]];
        
        if(l == 0)
        {
            // The input layer does not have a previous layer and does not have weights
            layer->previous_layer = NULL;
            layer->weights = NULL;
        }
        else
        {
            // Set pointer to the previous layer
            layer->previous_layer = layers[l-1];
            
            // Create a "matrix" of weights
            int weights_size = nodes_per_layer[l] * (nodes_per_layer[l - 1] + 1); // Note: we include the bias
            layer->weights = new double[weights_size];

            // Initialize weights using Gaussian distribution
            for(int x = 0;x < weights_size;x ++)
                layer->weights[x] = random_weight(layer);
        }
    }
}

double* Network::feed(double* input_values)
{
    // Copy the input values to the first layer
    for(int i = 0;i < layers[0]->amount_of_nodes;i ++)
        layers[0]->values[i] = input_values[i];
    
    // Do a chain of feeding forward
    for(int l = 1;l < amount_of_layers;l ++)
        layers[l]->evaluate();
    
    // Return the output of the last layer
    return layers[amount_of_layers - 1]->values;
}

void Network::train(double* input_values, double* desired_values)
{
    // First feed the input values
    feed(input_values);
    
    // Compute the deltas of the output layer
    Layer* output_layer = layers[amount_of_layers - 1];
    for(int i = 0;i < output_layer->amount_of_nodes;i ++)
    {
        double o = output_layer->values[i];
        output_layer->deltas[i] = (o - desired_values[i]) * o * (1.0 - o);
    }
    
    // Do a backwards chain of backpropagation
    for(int l = amount_of_layers - 1;l >= 1;l --)
    {
        layers[l]->backpropagate();
    }
}

Network::~Network()
{
    // Delete all layers
    for(int l = 0;l < amount_of_layers;l ++)
    {
        delete layers[l];
    }
    
    // Deallocate used memory
    delete layers;
}

// Save & Load functions
int Network::save(const char path[])
{
    // Create output
    std::ofstream out(path);
    
    // If something went wrong
    if(out < 0) return 0;
    
    // Write the size of the network / amount of layers
    out.write((char*) &amount_of_layers, sizeof(int));
    
    // Write the size of each layer
    for(int i = 0;i < amount_of_layers;i ++)
        out.write((char*) &(layers[i]->amount_of_nodes), sizeof(int));
    
    // Write each array of weights to the file
    for(int i = 1;i < amount_of_layers;i ++)
    {
        int n = layers[i]->amount_of_nodes * (layers[i - 1]->amount_of_nodes + 1) * sizeof(double);
        out.write((char*) (layers[i]->weights), n);
    }
    
    out.close();
    
    return 1;
}

Network::Network(const char path[])
{
    // Open file for reading
    std::ifstream in(path);
    
    // Read the size of the network / the amount of layers
    in.read((char*) &amount_of_layers, sizeof(int));
    
    // Read the sizes of each layer / the amount of nodes per layer
    int nodes_per_layer[amount_of_layers];
    in.read((char*) nodes_per_layer, amount_of_layers * sizeof(int));
    
    // Create an array of layers
    layers = new Layer*[amount_of_layers];
    
    for(int l = 0;l < amount_of_layers;l ++)
    {
        Layer* layer = layers[l] = new Layer();
        
        // Set amount of nodes
        layer->amount_of_nodes = nodes_per_layer[l];
        
        // Create an array of values and one of deltas
        layer->values = new double[nodes_per_layer[l]];
        layer->deltas = new double[nodes_per_layer[l]];
        
        if(l == 0)
        {
            // The input layer does not have a previous layer and does not have weights
            layer->previous_layer = NULL;
            layer->weights = NULL;
        }
        else
        {
            // Set pointer to the previous layer
            layer->previous_layer = layers[l-1];
            
            // Create a "matrix" of weights
            int weights_size = nodes_per_layer[l] * (nodes_per_layer[l - 1] + 1); // Note: we include the bias
            layer->weights = new double[weights_size];
            
            // Read the value of the weights from the file
            in.read((char*) (layers[l]->weights), layers[l]->amount_of_nodes * (layers[l - 1]->amount_of_nodes + 1) * sizeof(double));
        }
    }
    
    // Close the stream
    in.close();
}
