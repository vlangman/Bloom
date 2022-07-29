

const float PI = 3.141592655;


#define GlobalNeuronCount 1
#define GlobalConnectionCount 1
#define GlobalParameterCount 4

typedef uint NEURON_TYPE[4];
typedef uint NEURON_BASE_TYPE[5];


// #define Radians = PI/180.0;
struct Neuron
{
	uint neuronIndex;
	NEURON_TYPE type;
	NEURON_TYPE baseType;
	
	// public Gene gene { get; set; }
	// Parameter parameter[GlobalParameterCount]
	// Connection connections[GlobalConnectionCount];
};

struct Organism
{
	uint idx;
	uint alive;
	float2 orientation;
	float2 position;
	float4 color;

	uint neuronsStartIndex;
};

float RandomZeroToOne(uint seed){
    uint state = seed;
	state ^= 2747636419;
	state *= 2654435769;
	state ^= state >> 16;
	state *= 2654435769;
	state ^= state >> 16;
	return state / 4294967295.0;
}