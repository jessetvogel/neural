#define LAYER_TYPE_INPUT 0
#define LAYER_TYPE_HIDDEN 1
#define LAYER_TYPE_OUTPUT 2

class Layer
{
    public:
    
    // References to the layers before and after
    Layer* nextLayer;
    Layer* previousLayer;
    
    // About this layer
    int type;
    int size;
    Node** nodes;
    T_WEIGHT* weights;
    
    // Constructor
    Layer(int, int);
    
    // Destructor
    ~Layer();
    
    // Functions
    void setPreviousLayer(Layer*);
    T_WEIGHT getWeight(int, int);
    void setWeight(int, int, T_WEIGHT);
    Node* getNode(int);
    void input(T_NODE_VALUE*);
    void evaluate();
    T_NODE_VALUE* output();
    void backPropagate();
};
