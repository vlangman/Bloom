#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) volatile uniform image2D img_output;
layout(location = 2) uniform float deltaTime;
const vec2 resolution = vec2(800.0,800.0);

const float evaporationSpeed = 1;

void EvaporateTrail(uint index){
   // vec4 trailPixel = agents_in.agents[index].trailingPixel; 
    //agents_in.agents[index].trailingPixel.a = max(0, trailPixel.a - evaporationSpeed * deltaTime);
}


void main() 
{
}
