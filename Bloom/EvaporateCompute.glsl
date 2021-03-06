#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) writeonly uniform image2D img_output;
layout(location = 2) uniform float deltaTime;
layout(location = 5) uniform int speciesCount;
layout(location = 6) uniform vec2 resolution;

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
    vec4 sum  = vec4(0,0,0,0);
    int samples = 0;

    int blendSize = 1;
    int[] idArr = int[](speciesCount);
    //blend speed
    float averageBlendSpeed = 0;

    //evap speed
     
    for(int offsetX =-blendSize; offsetX <= blendSize; offsetX++)
    {
        for(int offsetY =-blendSize; offsetY <= blendSize; offsetY++)
        {
            int sampleX = pixel_coords.x + offsetX;
            int sampleY = pixel_coords.y + offsetY;

            if(sampleX >= 0 && sampleX < resolution.x && sampleY >= 0 && sampleY < resolution.y )
            {

                uint sampleIndex = uint(sampleY * resolution.x + sampleX);
                sum += trail_buffer.trailpixels[sampleIndex].pixelColour;
                averageBlendSpeed += trail_buffer.trailpixels[sampleIndex].SpeciesID_evapSpeed_blendSpeed.z;
                samples++;
                //inc the id in array so we can get that max later
                if(trail_buffer.trailpixels[sampleIndex].SpeciesID_evapSpeed_blendSpeed.x != -1)
                    idArr[int(trail_buffer.trailpixels[sampleIndex].SpeciesID_evapSpeed_blendSpeed.x)]++;

            }
       }

    }

    int idMax = 0;
    for(int i ; i < idArr.length(); i++)
    {
         if(idArr[i] < idMax)
            idMax = idArr[i];
    }

//    average blur colour;
    vec4 blurResult = sum/samples;


//    average blur speed based on all species
    averageBlendSpeed /= samples;

    //Blend colour over time
    sampleColour = mix(sampleColour, blurResult,   averageBlendSpeed  * deltaTime);
    //decrease brightness over time
    sampleColour.a =  max(0.0, sampleColour.a - trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed.y * deltaTime);
    //set colour in buffer


    //copy data across to new pixel
    trail_buffer.trailpixels[index].pixelColour = sampleColour;
    
    //set ID to the most present speciesID of sample size
    if(sampleColour.a > 0.01)
        trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed.x = trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed.x;
    else
//    remove species ID when too dim
        trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed.x = -1;


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