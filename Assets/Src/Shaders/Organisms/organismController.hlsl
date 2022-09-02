#include "../globals.hlsl"
#include "organism.hlsl"

#ifndef __ORGANISM_CONTROLLER
#define __ORGANISM_CONTROLLER

class OrganismController{


	void RandomMove(inout Organism organism, float moveSpeed)
	{
		uint idx = Index(organism.position);
		float seed = programState._seed * idx* programState._index + programState._time*10;
		organism.orientation = float2(RandomNormalized(seed), RandomNormalized(seed));

		organism.position += organism.orientation*moveSpeed*programState._deltaTime;
		ApplyResolutionBarrier(organism.position);
	}


	void ApplyResolutionBarrier(inout float2 position){
		position.x = clamp(position.x,0.0,programState._resolution.x-1);
		position.y = clamp(position.y,0.0,programState._resolution.y-1);
	}

};


#endif