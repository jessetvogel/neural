#include "node.h"

T_NODE_VALUE Node::activate(double _sum)
{
    // Uses the sigmoid function
    return (T_NODE_VALUE) (1.0 / (1.0 + std::exp(- _sum)));
}

