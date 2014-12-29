// Typedefs
typedef double T_WEIGHT;
typedef double T_NODE_VALUE;

class Input
{
    public:
    
    // Variables
    int size;
    T_NODE_VALUE* input;
    
    // Constructors
    Input(int, T_NODE_VALUE*);
    Input(char*);
    
    // Functions
    int save(char*);
    
    // Destructors
    ~Input();
};