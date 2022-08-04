#ifndef __PARAMETER
#define __PARAMETER

#define WEIGHT 0
#define LENGTH 1
#define SPEED 2
#define ANGLE 3
#define POLARITY 4
#define TIME 5,
#define STRENGTH  6
#define SIZE = 7



struct Parameter
{
	uint type;
	float value;
	// public Gene gene ;
};

interface IParameter {
	float Compute(Parameter parameter);
};

#endif // __PARAMETER