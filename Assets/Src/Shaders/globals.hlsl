
#ifndef __GLOBALS
#define __GLOBALS



const float PI = 3.141592655;


#define GlobalNeuronCount 2
#define GlobalConnectionCount 1
#define GlobalParameterCount 4

#define OrganismBufferSize Resolution.x*Resolution.y




struct ProgramState
{
	float _time;
	float _deltaTime;
	float4 _resolution;
	int _seed;
	uint3 _id;
	uint _index;
};
static ProgramState programState;



float RandomUnitSigned(uint seed){
    uint state = seed;
	state ^= 2747636419;
	state *= 2654435769;
	state ^= state >> 16;
	state *= 2654435769;
	state ^= state >> 16;
	return state / 4294967295.0;
}

uint2 Index2D(uint idx)
{
	return uint2(idx / programState._resolution.x, idx % programState._resolution.y);
}

uint Index(uint2 id){
	// x + y * num_cols
	return id.x + id.y * programState._resolution.x;
}

uint Index(float2 id){
	int2 normalized = int2(id.x,id.y);
	// x + y * num_cols
	return normalized.x + normalized.y * programState._resolution.x;
}

float RandomSign(float value, uint seed){
	if(RandomUnitSigned(value*seed) < 0.5)
		return -1*value;
	return value;
}

float RandomNormalized(uint seed)
{
	return RandomSign(RandomUnitSigned(seed),seed);
}


float RandomRange(uint seed ,float minimum, float maximum)
{
	return (RandomNormalized(seed) % (maximum - minimum + 1.0)) + minimum;
}

float2 DegToVec(float degrees)
{
	return float2(cos(degrees * PI / 180.0), sin(degrees * PI / 180.0));
}


void UpdateProgramState(
		uint3 id,
		float time, 
		float deltaTime, 
		float4 resolution, 
		int seed
	){
		programState._time = time;
		programState._deltaTime = deltaTime;
		programState._resolution = resolution;
		programState._seed = seed;
		programState._id = id;
		programState._index = Index(id.xy);
}




#endif // __GLOBALS