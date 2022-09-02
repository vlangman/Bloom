#include "../buffers.hlsl"
#include "../globals.hlsl"

#ifndef __PHEROMONE_CONTROLLER
#define __PHEROMONE_CONTROLLER

const float blurWeights[5] = {0.0,0.1945945946, 0.1216216216, 0.0540540541,0.0162162162};
class PheromoneController{
	
	float4 FastBlur(uint3 id, float2 dir)
	{
		//this will be our RGBA sum
		float4 sum = float4(0,0,0,0);
		
		//our original texcoord for this fragment
		uint2 tc = id.xy;
		
		//the amount to blur, i.e. how far off center to sample from 
		//1.0 -> blur by one pixel
		//2.0 -> blur by two pixels, etc.
		float blur = 1; 
		
		//the direction of our blur
		//(1.0, 0.0) -> x-axis blur
		//(0.0, 1.0) -> y-axis blur
		float hstep = dir.x;
		float vstep = dir.y;
		
		//apply blurring, using a 9-tap filter with predefined gaussian weights
		
		for(int i = 4; i >= 1; i--)
		{
			uint2 coord = uint2(tc.x - i*blur*hstep, tc.y - i*blur*vstep);
			uint index = Index(coord);
			sum += pheromoneBuffer[index].colour * blurWeights[i];
		};

		uint indexMid = Index(tc);
		sum +=  pheromoneBuffer[indexMid].colour * 0.2270270270;

		for(int i = 1; i <= 4; i++)
		{
			uint2 coord = uint2(tc.x + i*blur*hstep, tc.y + i*blur*vstep);
			uint index = Index(coord);
			sum += pheromoneBuffer[index].colour * blurWeights[i];
		};
	
		return sum;
	}


	void ShitBlur(uint3 id){


		if (id.x < 0 || id.x >= programState._resolution.x || id.y < 0 || id.y >= programState._resolution.y) {
			return;
		}
		
		float4 sum = float4(0,0,0,0);

		for(int x = -1; x < 2; x++)
		{
			for(int y = -1; y < 2; y++)
			{
				sum += pheromoneBuffer[Index(id.xy+int2(x,y))].colour;
			}
		}

		uint idx  = Index(id.xy);
		float4 average = sum /= 9;
		float4 originalCol = pheromoneBuffer[idx].colour;

		float diffuseWeight = 0.002;
		float4 result = originalCol * (1 - diffuseWeight) + average * (diffuseWeight);
		float decayRate = 0.02;
 		pheromoneBuffer[idx].colour = max(0, result - decayRate * programState._deltaTime);
	}

	void ReleasePheromone(Organism organism){
		uint worldPositionIndex = Index(organism.position);
		pheromoneBuffer[worldPositionIndex].colour = organism.colour;
	}

};


#endif