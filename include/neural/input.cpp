#include "input.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

Input::Input(int _size, T_NODE_VALUE* _input)
{
    // Save the size
    size = _size;
    
    // Copy the pointer to the input array
    input = _input;
}

Input::Input(char path[])
{
    // Open file
    std::ifstream in(path);
    
    // Read the amount of inputs
    in.read((char*) &size, sizeof(int));
    
    // Allocate space to save the input values, and then read them
    input = (T_NODE_VALUE*) malloc(size * sizeof(T_NODE_VALUE));
    in.read((char*) input, size * sizeof(T_NODE_VALUE));
    
    // Close the stream
    in.close();
}

int Input::save(char path[])
{
    std::ofstream out(path);
    
    // If something went wrong
    if(out < 0) return 0;
    
    // Write the amount of inputs
    out.write((char*) &size, sizeof(int));
    
    // Write each input value
    for(int i = 0;i < size;i ++) out.write((char*) &(input[i]), sizeof(T_NODE_VALUE));
    
    // Close the stream
    out.close();
    
    return 1;
}

Input::~Input()
{
    // Free the allocated space
    free(input);
}

