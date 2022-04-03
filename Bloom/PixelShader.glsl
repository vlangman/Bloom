#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
//layout(rgba32f, binding = 0) volatile uniform image2D img_output;
//layout(location = 2) uniform float deltaTime;
layout(location = 6) uniform vec2 resolution;
uniform vec2 speciesList[];



//struct Species {
//	vec4 speciesColour;
//	vec4 trailColour;
//	vec4 position;
//    vec4 angle_speed_turnSpeed;
//    ivec4 active_blendSize_sensorSize_speciesIndex;
//    vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
//    vec4 trailBehaviour_spawnChance;
//    vec4 uniqueIndex;
//};
//
//struct TrailPixel {
//	vec4 pixelColour;
//    vec4 SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex;
//    vec4 activeCount;
//};
//
//
////#enum TRAIL_BEHAVIOUR
//const int FOLLOW_SELF = 1;
//const int AVOID_ALL = 2;
//const int AVOID_OTHERS_FOLLOW_SELF = 3;
//const int FOLLOW_OTHERS = 4;
//
//layout( std430 , binding = 3 ) volatile buffer speciesBuffer
//{
//    Species species[];
//} species_in;
//
//layout( std430 , binding = 4 ) volatile buffer trailBuffer
//{
//    TrailPixel trailpixels[];
//} trail_buffer;

float hashFunc(float stateIN)
{
    uint state = uint(stateIN);
	state ^= 2747636419;
	state *= 2654435769;
	state ^= state >> 16;
	state *= 2654435769;
	state ^= state >> 16;
	return state / 4294967295.0;
}








void main() 
{
    // get index in global work group i.e x,y position
    uint index = uint(gl_GlobalInvocationID.y  * resolution.x + gl_GlobalInvocationID.x);


}

