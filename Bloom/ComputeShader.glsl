#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) volatile uniform image2D img_output;
layout(location = 2) uniform float deltaTime;
const vec2 resolution = vec2(1000.0,1000.0);

layout(std430, binding = 4) buffer trailbuffer
{
    vec4 trailpixels[];
} trail_buffer;


const float evaporationSpeed = 0.5;
const float blendSpeed= 2;
const int blendSize = 1;
void EvaporatePixel(ivec2 pixel_coords){
    
    vec4 sampleColour = trail_buffer.trailpixels[int(pixel_coords.y * resolution.x + pixel_coords.x)];
//    // we now need to blur this pixel by surroundings
//    vec4 sum = sampleColour;
//    int samples = 0;
//  
//    for(int offsetX =-blendSize; offsetX <= blendSize; offsetX++)
//    {
//        for(int offsetY =-blendSize; offsetY <= blendSize; offsetY++)
//        {
//            int sampleX = pixel_coords.x + offsetX;
//            int sampleY = pixel_coords.y + offsetY;
//        
//            if(sampleX >= 0 && sampleX < resolution.x && sampleY >= 0 && sampleY < resolution.y )
//            {
//                sum += trail_buffer.trailpixels[int(sampleY * resolution.x + sampleX)];
//                samples++;
//            }
//       }
//
//    }
//   
//    vec4 blurResult = sum/samples;
//    vec4 diffuseValue = mix(sampleColour, blurResult, blendSpeed * deltaTime);
//    sampleColour = diffuseValue;

//    sampleColour.a =  max(0.0, sampleColour.a - evaporationSpeed * deltaTime);

    barrier();
    trail_buffer.trailpixels[int(pixel_coords.y * resolution.x + pixel_coords.x)] = sampleColour;
    //output sample colour
    imageStore(img_output, pixel_coords,sampleColour);
}


void main( )
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);
    EvaporatePixel(pixel_coords);

}