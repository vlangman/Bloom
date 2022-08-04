
#ifndef __ORGANISM
#define __ORGANISM

struct Organism
{
	uint idx;
	uint alive;
	float2 orientation;
	float2 position;
	float4 color; 

	uint neuronsStartIndex;
};


#endif