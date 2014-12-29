// Typedefs
typedef double T_WEIGHT;
typedef double T_NODE_VALUE;

class Network
{
    public:
    
    // "Constants"
    static double trainingConstant;
    int size;
	Layer** layers;
	
    // Constructors
    Network(int, int*);
    Network(char*);
	
    // Destructor
    ~Network();
    
    // Functions
    void init(int, int*);
    Layer* getLayer(int);
    T_NODE_VALUE* input(T_NODE_VALUE*);
    T_NODE_VALUE* input(Input*);
    void train(T_NODE_VALUE*, T_NODE_VALUE*);
    int save(char*);
};