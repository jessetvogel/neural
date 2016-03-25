#include "functions.h"

double gaussian(double mu, double sigma) // Thank you internet!
{
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0 * 3.14159265358979323846;
    
	static double z0, z1;
	static bool generate;
	generate = !generate;
    
	if (!generate)
        return z1 * sigma + mu;
    
	double u1, u2;
	do
    {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    }
	while ( u1 <= epsilon );
    
	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

double random_weight(Layer* layer)
{
    return gaussian(0.0, 1.0 / sqrt(layer->previous_layer->amount_of_nodes + 1)); // TODO: misschien nog een keertje naar kijken? Het werkt opzich wel, dus ik laat het eerst maar staan...
//    return (double) rand() / RAND_MAX * 2 - 1.0; // Alternative
}
