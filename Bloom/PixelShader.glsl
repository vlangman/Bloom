#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) volatile uniform image2D img_output;
layout(location = 2) uniform float deltaTime;
const vec2 resolution = vec2(800.0,800.0);

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

//#enum TRAIL_BEHAVIOUR
const int FOLLOW_SELF = 1;
const int AVOID_ALL = 2;
const int AVOID_OTHERS_FOLLOW_SELF = 3;
const int FOLLOW_OTHERS = 4;

layout( std430 , binding = 3 ) volatile buffer speciesBuffer
{
    Species species[];
} species_in;

layout( std430 , binding = 4 ) volatile buffer trailBuffer
{
    TrailPixel trailpixels[];
} trail_buffer;

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


float sense(Species speciesIn, float sensorAngleOffset){
    float sensorAngle = speciesIn.angle_speed_turnSpeed.x + sensorAngleOffset;
    vec2 sensorDir = vec2(cos(sensorAngle), sin(sensorAngle));
    ivec2 sensorCenter = ivec2(speciesIn.position.xy + sensorDir * speciesIn.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z);
    float sum = -1;
    int samples = 0;
    int sensSize = speciesIn.active_blendSize_sensorSize_speciesIndex.z;
//    bool invalidMove = false;

//    species_in.species[uint(gl_GlobalInvocationID.y * resolution.x +gl_GlobalInvocationID.x)].evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.w += .1 *deltaTime;
//   species_in.species[uint(gl_GlobalInvocationID.y * resolution.x +gl_GlobalInvocationID.x)].evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z -= 0.01*deltaTime;

    for(int offsetX = -sensSize; offsetX <= sensSize; offsetX++)
    {
        for(int offsetY = -sensSize; offsetY <= sensSize; offsetY++)
        {
            ivec2 pos = sensorCenter + ivec2(offsetX,offsetY);
            //if within bounds of texture/screen
            if(pos.x >= 0 && pos.x < resolution.x && pos.y >= 0 && pos.y < resolution.y)
            {

                //  int SpeciesID =  int(trail_buffer.trailpixels[int(pos.y * resolution.x + pos.x)].SpeciesID_evapSpeed_blendSpeed.x);
                float alpha = trail_buffer.trailpixels[int(pos.y * resolution.x + pos.x)].pixelColour.a;

                //if (FOLLOW_SELF ) only add the alpha of species == species ID 
                if(speciesIn.trailBehaviour.x == FOLLOW_SELF )
                {
     
                      sum += alpha;
                      samples++;

                }

                //if (AVOID_ALL ) sum alpha and return , we will use the lowest value of all sense results
//                if(speciesIn.trailBehaviour.x == AVOID_ALL )
//                {
//                    //add the sum of dark space (1 - alpha)
//                    sum +=  1.0 - alpha;
//                    samples++;
//                }

////                //if (AVOID_OTHERS_FOLLOW_SELF) we first want to avoid others before it follows itself
//                if(speciesIn.trailBehaviour.x == AVOID_OTHERS_FOLLOW_SELF)
//                {
//   
//                     //if other species add it to the 
//                     if(SpeciesID != speciesIn.active_blendSize_sensorSize_speciesIndex.w && SpeciesID != -1)
//                        invalidMove = true;
//
//                     else 
//                        sum += alpha;
//                     samples++;
//                }
//        
            }
        }
    }

//    if(invalidMove)
//    {
//        return -1;
//    }

    return sum/samples;
}

Species SelectDirection( Species speciesIn, float randomSeed ){
    

    //weights come out depending on the species targets (self/Others)
    float weightFoward = sense(speciesIn, 0);
    float weightLeft = sense(speciesIn, speciesIn.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.w);
    float weightRight = sense(speciesIn, -speciesIn.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.w);



    float randomSteerStrength = 0.1;
   //turn random
    if( weightFoward <= weightLeft && weightFoward <=  weightRight)
      speciesIn.angle_speed_turnSpeed.x += (randomSeed - 0.5) * 2 *  speciesIn.angle_speed_turnSpeed.z * deltaTime  * randomSteerStrength;
//    continue in direction
    if(weightFoward >= weightLeft && weightFoward >= weightRight)
        return speciesIn;
  
    //turn right
    else if( weightRight > weightLeft)
        speciesIn.angle_speed_turnSpeed.x -= speciesIn.angle_speed_turnSpeed.z * deltaTime;
    //turn left
    else if (weightLeft > weightRight)
        speciesIn.angle_speed_turnSpeed.x += speciesIn.angle_speed_turnSpeed.z * deltaTime;


    return speciesIn;
}


void main() 
{
    // get index in global work group i.e x,y position
    uint index = uint(gl_GlobalInvocationID.y  * resolution.x + gl_GlobalInvocationID.x);
    //filp x and y for more noisy / even spawn seed
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);


    Species species = species_in.species[index];
    ivec2 originalPos = ivec2(species.position.xy);

    barrier();
    
    if(species.active_blendSize_sensorSize_speciesIndex.x == 0)
        return;
    
          

  float Random = hashFunc(gl_GlobalInvocationID.y  * resolution.x + gl_GlobalInvocationID.x);
  species = SelectDirection( species,Random);
  vec2 direction = vec2(cos(species.angle_speed_turnSpeed.x), sin(species.angle_speed_turnSpeed.x));
  vec2 newPos = species.position.xy + direction * species.angle_speed_turnSpeed.y * deltaTime;
  if(newPos.x < 0 || newPos.x >= resolution.x  || newPos.y < 0 || newPos.y >= resolution.y )
  {
    newPos.x = min( max(1,newPos.x) , resolution.x - 1 );
    newPos.y = min( max(1,newPos.y) , resolution.y -1 );
    species.angle_speed_turnSpeed.x += Random * 2 * 3.141592654 ;
    if( mod(90,species.angle_speed_turnSpeed.x) == 0)
    {
         species.angle_speed_turnSpeed.x += Random * (150) + 30  ;
    }
//    species.speciesColour = vec4(hashFunc(index),hashFunc(index),hashFunc(index),1);
  }
    
  //write the new data in buffer
  species_in.species[index].position.xy = newPos;
  species_in.species[index].angle_speed_turnSpeed.x = species.angle_speed_turnSpeed.x;
//  species_in.species[index].speciesColour = species.speciesColour;
  barrier();
  //write new pos to trail buffer
  uint originalPosIndex = uint(originalPos.y * resolution.x + originalPos.x);
  trail_buffer.trailpixels[originalPosIndex].pixelColour = species.trailColour;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed.x = species.active_blendSize_sensorSize_speciesIndex.w;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed.y = species.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.x;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed.z = species.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.y;
  ivec2 location = ivec2(newPos.x, newPos.y);
  barrier();

//   output to a specific pixel in the image
  imageStore(img_output, location, species.speciesColour);

}

