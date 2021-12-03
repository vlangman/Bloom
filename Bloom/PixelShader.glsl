#version 460 core
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) volatile uniform image2D img_output;
layout(location = 2) uniform float deltaTime;
layout(location = 6) uniform vec2 resolution;

struct Species {
	vec4 speciesColour;
	vec4 trailColour;
	vec4 position;
    vec4 angle_speed_turnSpeed;
    ivec4 active_blendSize_sensorSize_speciesIndex;
    vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
    vec4 trailBehaviour_spawnChance;
    vec4 uniqueIndex;
};

struct TrailPixel {
	vec4 pixelColour;
    vec4 SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex;
    vec4 activeCount;
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




float lineDetection(int x0, int y0, int x1, int y1, Species speciesIn) 
{

   int dx = abs(x1 - x0);
   int dy = abs(y1 - y0);
   int sx = (x0 < x1) ? 1 : -1;
   int sy = (y0 < y1) ? 1 : -1;
   float err = dx - dy;

   float sum = 0;
   int samples = 0;
   while(true) {

     
//        imageStore(img_output, ivec2(x0,y0), vec4(1,0,0,0.5));
        

      uint index = uint(y0 * resolution.x + x0);

      if(x0 >= 0 && x0 < resolution.x && y0 >= 0 && y0 < resolution.y)
      {
            float alpha = trail_buffer.trailpixels[index].pixelColour.a;

            //if trail it lays is not its own trail 
            if(trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.w != speciesIn.uniqueIndex.x)
            {

                sum += alpha;

////                 #DEBUG PRINT
//                if((trail_buffer.trailpixels[index].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.x == speciesIn.active_blendSize_sensorSize_speciesIndex.w) && alpha > 0.01)
//                    imageStore(img_output, ivec2(x0,y0), vec4(0,1,0,1));
////                #DEBUG PRINT
            }
            samples++;
      }


      if ((x0 == x1) && (y0 == y1))
        break;
      float e2 = 2*err;
      if (e2 > -dy) { err -= dy; x0  += sx; }
      if (e2 < dx) { err += dx; y0  += sy; }
   }
   return sum/samples;
   
}

float sense(Species speciesIn, float sensorAngleOffset){


   
   float sum = 0;
    int samples = 0;
    int sensSize = speciesIn.active_blendSize_sensorSize_speciesIndex.z;

    vec2 sensorDir = vec2(cos(sensorAngleOffset), sin(sensorAngleOffset));
    vec2 sensorCenter = vec2(speciesIn.position.xy + sensorDir * speciesIn.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z);

   return lineDetection(int(speciesIn.position.x),int(speciesIn.position.y),int(sensorCenter.x),int(sensorCenter.y),speciesIn);

    for(int offsetX = -sensSize; offsetX <= sensSize; offsetX++)
    {
        for(int offsetY = -sensSize; offsetY <= sensSize; offsetY++)
        {
            ivec2 pos = ivec2(sensorCenter) + ivec2(offsetX,offsetY);
            //if within bounds of texture/screen
            if(pos.x >= 0 && pos.x < resolution.x && pos.y >= 0 && pos.y < resolution.y)
            {
                float alpha = trail_buffer.trailpixels[int(pos.y * resolution.x + pos.x)].pixelColour.a;
                int SpeciesID =  int(trail_buffer.trailpixels[int(pos.y * resolution.x + pos.x)].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.x);
                //if (FOLLOW_SELF ) only add the alpha of species == species ID   
//                imageStore(img_output, ivec2(pos.x,pos.y), vec4(0,1,0,1));
                if(speciesIn.trailBehaviour_spawnChance.x == FOLLOW_SELF )
                {
                    if(SpeciesID != speciesIn.active_blendSize_sensorSize_speciesIndex.a)
                    {
                        sum -= alpha;
                    }
                    else
                    {
                               
//                        imageStore(img_output, ivec2(pos.x,pos.y), vec4(0,1,0,1));
                        sum += alpha;
                    }
                    samples++;
                }
                else if(speciesIn.trailBehaviour_spawnChance.x == AVOID_ALL )
                {
                       sum += 1.0 - alpha;
                }
    
            }
            //if out of texture bounds add -1 so it steers back into frame
            else{
                sum -= 1;
            }
            samples++;
        }
    }
    return sum/samples;

//    bool invalidMove = false;

//   species_in.species[uint(gl_GlobalInvocationID.y * resolution.x +gl_GlobalInvocationID.x)].evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z -= 0.01*deltaTime;



//    if(invalidMove)
//    { 
//    }
 

}



Species SelectDirection( Species speciesIn, float randomSeed ){



    //weights come out depending on the species targets (self/Others)
    //start with species direcion
    float weightFoward = sense(speciesIn, speciesIn.angle_speed_turnSpeed.x);
    float weightLeft = sense(speciesIn, speciesIn.angle_speed_turnSpeed.x + speciesIn.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.w);
    float weightRight = sense(speciesIn, speciesIn.angle_speed_turnSpeed.x -speciesIn.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.w);


    float randomSteerStrength = 0.1;
//   //turn random
//    if( weightFoward <= weightLeft && weightFoward <=  weightRight)
//      speciesIn.angle_speed_turnSpeed.x += (randomSeed - 0.5) * 2 *  speciesIn.angle_speed_turnSpeed.z * deltaTime  * randomSteerStrength;
//    continue in direction
    if(weightFoward >= weightLeft && weightFoward >= weightRight)
        return speciesIn;
    //turn right
    else if( weightRight > weightLeft)
        speciesIn.angle_speed_turnSpeed.x -= speciesIn.angle_speed_turnSpeed.z * deltaTime;
    //turn left
    else if (weightLeft > weightRight)
        speciesIn.angle_speed_turnSpeed.x += speciesIn.angle_speed_turnSpeed.z * deltaTime;

//
    return speciesIn;
}




void main() 
{
    // get index in global work group i.e x,y position
    uint index = uint(gl_GlobalInvocationID.y  * resolution.x + gl_GlobalInvocationID.x);

    Species species = species_in.species[index];
    ivec2 originalPos = ivec2(species.position.xy);
    trail_buffer.trailpixels[index].activeCount.x = 0;


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
    species.angle_speed_turnSpeed.x += Random * 2 * 3.141592654;
  }
    
  //write the new data in buffer
  species_in.species[index].position.xy = newPos;
  species_in.species[index].angle_speed_turnSpeed.x = species.angle_speed_turnSpeed.x;
//  species_in.species[index].speciesColour = species.speciesColour;
  barrier();
  //write new pos to trail buffer
  uint originalPosIndex = uint(originalPos.y * resolution.x + originalPos.x);
  trail_buffer.trailpixels[originalPosIndex].pixelColour = species.trailColour;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.x = species.active_blendSize_sensorSize_speciesIndex.w;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.y = species.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.x;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.z = species.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.y;
  trail_buffer.trailpixels[originalPosIndex].SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex.w = species.uniqueIndex.x;
  ivec2 location = ivec2(newPos.x, newPos.y);

  trail_buffer.trailpixels[uint(location.y * resolution.x + location.x)].activeCount.x = 1;
     
 //   species_in.species[uint(gl_GlobalInvocationID.y * resolution.x +gl_GlobalInvocationID.x)].evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.a += 0.1 *deltaTime;
//    species_in.species[uint(gl_GlobalInvocationID.y * resolution.x +gl_GlobalInvocationID.x)].evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z += 0.01 *deltaTime;
//     species_in.species[uint(gl_GlobalInvocationID.y * resolution.x +gl_GlobalInvocationID.x)].trailColour.y += 1 *deltaTime;
  barrier();

//   output to a specific pixel in the image
  imageStore(img_output, location, species.speciesColour);

}

