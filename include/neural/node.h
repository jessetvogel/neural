#include <cmath>

// Typedefs
typedef double T_WEIGHT;
typedef double T_NODE_VALUE;

class Node
{
    public:
    
    T_NODE_VALUE output; // Contains the last output calculated
    T_NODE_VALUE error; // Contains the error for the desired output
    
    static T_NODE_VALUE activate(double);
};
