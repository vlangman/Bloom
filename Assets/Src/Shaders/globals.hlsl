
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
};
static ProgramState programState;

void UpdateProgramState(float time, float deltaTime, float4 resolution){
	programState._time = time;
	programState._deltaTime = deltaTime;
	programState._resolution = resolution;
}



float RandomZeroToOne(uint seed){
    uint state = seed;
	state ^= 2747636419;
	state *= 2654435769;
	state ^= state >> 16;
	state *= 2654435769;
	state ^= state >> 16;
	return state / 4294967295.0;
}


float RandomSign(float value, uint seed){
	if(RandomZeroToOne(value*seed) < 0.5)
		return -1*value;
	return value;
}

float RandomNormalized(uint seed)
{
	return RandomSign(RandomZeroToOne(seed),seed);
}


float RandomRange(uint seed ,float minimum, float maximum)
{
	return (RandomNormalized(seed) % (maximum - minimum + 1.0)) + minimum;
}

float2 DegToVec(float degrees)
{
	return float2(cos(degrees * PI / 180.0), sin(degrees * PI / 180.0));
}


#endif // __GLOBALS