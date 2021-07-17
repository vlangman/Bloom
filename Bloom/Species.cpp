#pragma once

#include "glm.hpp"
#include <time.h>
#include <random>
#include <iostream>
#include <string>


struct TrailPixel {
	TrailPixel() {
		pixelColour = glm::vec4(0, 0, 0, 0);
		SpeciesID_evapSpeed_blendSpeed = glm::vec4(-1, 0, 0, 0);
	}

	glm::vec4 pixelColour;
	glm::vec4 SpeciesID_evapSpeed_blendSpeed;
};

struct Species {
	Species() {
		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing = glm::vec4(0,0,0,0);
		active_blendSize_sensorSize_speciesIndex = glm::vec4(0, 0, 0, -1);
	};
	Species(
		glm::vec4 s_Colour,
		glm::vec4 s_trailColour,
		glm::vec4 s_position,
		glm::vec4 s_angle_speed_turnSpeed,
		glm::ivec4 s_active_blendSize_sensorSize,
		glm::vec4 s_evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing,
		glm::vec4 s_trailBehaviour
	) {
		position = s_position;
		speciesColour = s_Colour;
		trailColour = s_trailColour;
		active_blendSize_sensorSize_speciesIndex = s_active_blendSize_sensorSize;
		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing = s_evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
		angle_speed_turnSpeed = s_angle_speed_turnSpeed;
		trailBehaviour = s_trailBehaviour; 
    }
		glm::vec4 speciesColour;
		glm::vec4 trailColour;
		glm::vec4 position;
		glm::vec4 angle_speed_turnSpeed;
		glm::ivec4 active_blendSize_sensorSize_speciesIndex;
		glm::vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
		glm::vec4 trailBehaviour;
};


enum TRAIL_BEHAVIOUR
{
	FOLLOW_SELF = 1, 
	AVOID_ALL = 2,
	AVOID_OTHERS_FOLLOW_SELF = 3,
	FOLLOW_OTHERS = 4
};

class SpeciesFactory
{
private:

public:
	SpeciesFactory(void) {};
	~SpeciesFactory(void) {};
	Species * _agent;

	int speciesIndex = 0;
	 
	//[ 0 - 150]
	float sensorOffsetDistance = 2;
	//[ 1 - 179]
	float sensorAngleSpacing = 25;
	//[ 1 - 2 ]
	int sensorSize = 1;
	//[ 0.02 - 5]
	float evaporationSpeed = 1 ;
	//max(evaporationSpeed, [0.02 - 10])
	float blendSpeed = 15;
	//[ 1 - 2 ]
	int blendSize = 1;

	//[ 5 - 120 ]
	float turnSpeed = 25;
	//[ 5 - 120 ]
	float moveSpeed = 55;
	float angle = rand() % 1 * 360;

	glm::vec4 speciesColour = glm::vec4(0,0,0,0);
	glm::vec4 trailColour = glm::vec4(0,0,0,0);
	glm::vec4 position= glm::vec4(0,0,0,0);
	glm::vec4 angle_speed_turnSpeed = glm::vec4(angle,moveSpeed,turnSpeed,0);
	glm::ivec4 active_blendSize_sensorSize_speciesIndex = glm::ivec4(0,blendSize,sensorSize, speciesIndex);
	glm::vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing = glm::vec4(evaporationSpeed, blendSpeed, sensorOffsetDistance, sensorAngleSpacing);
	glm::vec4 trailBehaviour = glm::vec4(0, 0, 0, 0);



	Species * GenerateSpeciesType() {


		RandomizeSpeciesColour();
		RandomTrailBehaviour(); 
		RandomizeAngle();
		//RandomStationary();
		_agent = new Species(
			speciesColour,
			trailColour,
			position,
			angle_speed_turnSpeed,
			active_blendSize_sensorSize_speciesIndex,
			evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing,
			trailBehaviour
		);
	 
		speciesIndex++;
		active_blendSize_sensorSize_speciesIndex.w++;
		return _agent;
	}

	void RandomizeAngle()
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0.0, 360 );

		angle_speed_turnSpeed.x = dis(gen);
	}

	void RandomizeSpeciesColour()
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		//srand(seed);
		std::uniform_real_distribution<> dis(0.0, INT_MAX);
		float r = dis(gen)/ INT_MAX;
		float g = dis(gen) / INT_MAX;
		float b = dis(gen) / INT_MAX;
		speciesColour = glm::vec4(r, g, b, 1);
		//speciesColour = glm::vec4(1,1,1,1);
		trailColour = speciesColour;
	}
	
	void RandomTrailBehaviour() {
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0, 1.0);
		int index = int(dis(gen) + 1);
 		trailBehaviour.x = static_cast<TRAIL_BEHAVIOUR>(index);
		//trailBehaviour.x = static_cast<TRAIL_BEHAVIOUR>(TRAIL_BEHAVIOUR::AVOID_ALL);
  		std::cout << "Behaviour type: " + std::to_string(index) << std::endl;
	}

	//bool done = false;
	//void RandomStationary() {
	//	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	//	std::uniform_real_distribution<> dis(0.0,1.0 );
	//	if(!done);
	//	{  
	//		done = true;
	//		std::cout << "WOOOOOAH NELLY " << std::endl;
	//		angle_speed_turnSpeed.y = 0;
	//	}
	//}

};

