#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) writeonly uniform image2D img_output;
layout(location = 2) uniform float deltaTime;
const vec2 resolution = vec2(800.0,800.0);
layout(location = 5) uniform int speciesCount;

struct Species {
	vec4 speciesColour;
	vec4 trailColour;
	vec4 position;
    vec4 angle_speed_turnSpeed;
    ivec4 active_blendSize_sensorSize_speciesIndex;
    vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
    vec4 trailBehaviour;
};

struct TrailPixel {
	vec4 pixelColour;
    vec4 SpeciesID_evapSpeed_blendSpeed;
};

layout( std430 , binding = 4 ) volatile buffer trailBuffer
{
    TrailPixel trailpixels[];
} trail_buffer;


void EvaporatePixel(ivec2 pixel_coords){
    uint index = uint(pixel_coords.y * resolution.x + pixel_coords.x);
    vec4 sampleColour = trail_buffer.trailpixels[index].pixelColour;
    barrier();
    // we now need to blur this pixel by surroundings
    vec4 sum = sampleColour;
    int samples = 1;

    int blendSize = 1;
    int[] idArr = int[](speciesCount);
    //blend speed
    float averageBlendSpeed = 0;
    int blendSamples = 0;
    //evap speed
     
//    for(int offsetX =-blendSize; offsetX <= blendSize; offsetX++)
//    {
//        for(int offsetY =-blendSize; offsetY <= blendSize; offsetY++)
//        {
//            int sampleX = pixel_coords.x + offsetX;
//            int sampleY = pixel_coords.y + offsetY;
//
//            if(sampleX >= 0 && sampleX < resolution.x && sampleY >= 0 && sampleY < resolution.y )
//            {
//                uint sampleIndex = uint(sampleY * resolution.x + sampleX);
//                sum += trail_buffer.trailpixels[sampleIndex].pixelColour;
//                averageBlendSpeed += trail_buffer.trailpixels[sampleIndex].SpeciesID_evapSpeed_blendSpeed.z;
////                if(trail_buffer.trailpixels[sampleIndex].SpeciesID_evapSpeed_blendSpeed.x >= 0)
//                blendSamples++;
//
//                //inc the id in array so we can get that max later
//                idArr[int(trail_buffer.trailpixels[uint(sampleY * resolution.x + sampleX)].SpeciesID_evapSpeed_blendSpeed.x)]++;
//                samples++;
//            }
//       }
//
//    }


    int idMax = 0;
    for(int i ; i < idArr.length(); i++)
    {
         if(idArr[i] < idMax)
            idMax = idArr[i];
    }

    //average blur colour;
    vec4 blurResult = sum/samples;

        //average blur speed based on all species
    averageBlendSpeed/= blendSamples;

    //Blend colour over time
//    sampleColour = mix(sampleColour, blurResult,  trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed.z  * deltaTime);
    //decrease brightness over time
    sampleColour.a =  max(0.0, sampleColour.a - trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed.y * deltaTime);
    //set colour in buffer
    trail_buffer.trailpixels[uint(pixel_coords.y * resolution.x + pixel_coords.x)].pixelColour = sampleColour;

    
    //set ID to the most present speciesID of sample size
    if(sampleColour.a > 0.01)
        trail_buffer.trailpixels[uint(pixel_coords.y * resolution.x + pixel_coords.x)].SpeciesID_evapSpeed_blendSpeed.x = idMax;
    else
    //remove species ID when too dim
        trail_buffer.trailpixels[uint(pixel_coords.y * resolution.x + pixel_coords.x)].SpeciesID_evapSpeed_blendSpeed.x = -1;

    barrier();
      //output trailmap
    imageStore(img_output, pixel_coords, sampleColour );
}



void main( )
{

    //clear screen
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);
    EvaporatePixel(pixel_coords);

}