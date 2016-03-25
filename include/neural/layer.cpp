#include "layer.h"
#include "network.h"
#include "functions.h"

void Layer::evaluate()
{
    // Update each value
    for(int j = 0;j < amount_of_nodes;j ++)
    {
        // Compute the weighted sum
        int previous_layer_amount_of_nodes = previous_layer->amount_of_nodes;
        double net = weights[(j + 1) * (previous_layer_amount_of_nodes + 1) - 1]; // Start with the bias = 1 * weights["bias"]
        for(int i = 0;i < previous_layer_amount_of_nodes;i ++)
        {
            net += weights[j * (previous_layer_amount_of_nodes + 1) + i] * previous_layer->values[i];
        }
        
        // Apply the sigmoid function
        values[j] = sigmoid(net);
    }
}

void Layer::backpropagate()
{
    int previous_layer_amount_of_nodes = previous_layer->amount_of_nodes;
    for(int i = 0;i < previous_layer_amount_of_nodes;i ++)
    {
        // Compute the deltas of the previous layer
        double delta_net = 0.0;
        for(int j = 0;j < amount_of_nodes;j ++)
        {
            delta_net += weights[j * (previous_layer_amount_of_nodes + 1) + i] * deltas[j];
        }
        
        double o = previous_layer->values[i];
        previous_layer->deltas[i] = delta_net * o * (1.0 - o);
        
        // Update weights (note that we do not need the deltas of the previous layer, but it may be handy to have computed the deltas of the previous layer before we change the values of the weights)
        for(int j = 0;j < amount_of_nodes; j++)
        {
            weights[j * (previous_layer_amount_of_nodes + 1) + i] -= Network::alpha * deltas[j] * o;
        }
    }
    
    // Lastly, update the weights corresponding to the bias
    for(int j = 0;j < amount_of_nodes;j ++)
    {
        weights[(j + 1) * (previous_layer_amount_of_nodes + 1) - 1] -= Network::alpha * deltas[j];
    }
}

Layer::~Layer()
{
    // Deallocate used memory
    if(weights != NULL) delete weights;
    
    delete values;
    delete deltas;
}
