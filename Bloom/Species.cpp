#pragma once

#include "glm.hpp"
#include <time.h>
#include <random>
#include <iostream>
#include <string>


struct TrailPixel {
	TrailPixel() {
		pixelColour = glm::vec4(0, 0, 0, 0);
		SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex = glm::vec4(-1, 0, 0, 0);
		activeCount = glm::vec4(0,0,0,0);
	}

	glm::vec4 pixelColour;
	glm::vec4 SpeciesID_evapSpeed_blendSpeed_speciesArrayIndex;
	glm::vec4 activeCount;
};

struct Species {
	Species() {
		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing = glm::vec4(0,0,0,0);
		active_blendSize_sensorSize_speciesIndex = glm::vec4(0, 0, 1, -1);
		uniqueIndex = glm::vec4(-1);
	};
	Species(
		glm::vec4 s_Colour,
		glm::vec4 s_trailColour,
		glm::vec4 s_position,
		glm::vec4 s_angle_speed_turnSpeed,
		glm::ivec4 s_active_blendSize_sensorSize_speciesID,
		glm::vec4 s_evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing,
		glm::vec4 s_trailBehaviour_spawnChance

	) {
		position = s_position;
		speciesColour = s_Colour;
		trailColour = s_trailColour;
		active_blendSize_sensorSize_speciesIndex = s_active_blendSize_sensorSize_speciesID;
		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing = s_evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
		angle_speed_turnSpeed = s_angle_speed_turnSpeed;
		trailBehaviour_spawnChance = s_trailBehaviour_spawnChance; 
    }
		glm::vec4 speciesColour;
		glm::vec4 trailColour;
		glm::vec4 position;
		glm::vec4 angle_speed_turnSpeed;
		glm::ivec4 active_blendSize_sensorSize_speciesIndex;
		glm::vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing;
		glm::vec4 trailBehaviour_spawnChance;
		glm::vec4 uniqueIndex;
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
	 
	//[ 0 - 120]
	float sensorOffsetDistance = 2;
	//[ 1 - 150]
	float sensorAngleSpacing = 25;
	//[ 1 - 2 ]
	int sensorSize = 1;
	//[ 0.1 - 5]
	float evaporationSpeed = 2;
	//max(evaporationSpeed, [0.1 - 10])
	float blendSpeed = 1;
	//[ 1 - 2 ]
	int blendSize = 1;

	float spawnChance = 0.5;

	//[ 5 - 120 ]
	float turnSpeed = 25;
	//[ 5 - 75 ]
	float moveSpeed = 15;
	float angle = rand() % 1 * 360;

	glm::vec4 speciesColour = glm::vec4(0,0,0,0);
	glm::vec4 trailColour = glm::vec4(0,0,0,0);
	glm::vec4 position= glm::vec4(0,0,0,0);
	glm::vec4 angle_speed_turnSpeed = glm::vec4(angle,moveSpeed,turnSpeed,0);
	glm::ivec4 active_blendSize_sensorSize_speciesIndex = glm::ivec4(0,blendSize,sensorSize, 0);
	glm::vec4 evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing = glm::vec4(evaporationSpeed, blendSpeed, sensorOffsetDistance, sensorAngleSpacing);
	glm::vec4 trailBehaviour_spawnChance = glm::vec4(0, spawnChance, 0, 0);

	Species * GenerateSpeciesType() {


		RandomizeSpeciesColour();
		RandomtrailBehaviour_spawnChance(); 
		RandomizeAngle();
		RandomMoveSpeed();
		RandomTurnSpeed();
		RandomSensorParams(); 
		RandomTrailParams();
		//RandomStationary();
		
		_agent = new Species(
			speciesColour,
			trailColour,
			position,
			angle_speed_turnSpeed,
			active_blendSize_sensorSize_speciesIndex,
			evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing,
			trailBehaviour_spawnChance
		);
		//increment the species index
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
	
	void RandomtrailBehaviour_spawnChance() {
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0, 2.0);
		int index = int(dis(gen) + 1);
 		trailBehaviour_spawnChance.x = static_cast<TRAIL_BEHAVIOUR>(index);
		//trailBehaviour_spawnChance.x = static_cast<TRAIL_BEHAVIOUR>(TRAIL_BEHAVIOUR::FOLLOW_SELF);
  		std::cout << "Behaviour type: " + std::to_string(index) << std::endl;
	}

	void RandomMoveSpeed()
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(6, 75);
		//random move speed
		int speed = int(dis(gen) + 1);


		//angle_speed_turnSpeed.y = speed;
		//angle_speed_turnSpeed.y = 0;

		std::cout << "MoveSpeed: " + std::to_string(angle_speed_turnSpeed.x) << std::endl;
	}

	void RandomTurnSpeed()
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(5, 100);
		//random move speed
		int turnSpeed = int(dis(gen) + 1);

		angle_speed_turnSpeed.z = turnSpeed;
		std::cout << "TurnSpeed: " + std::to_string(turnSpeed) << std::endl;
	
	}


	void RandomSensorParams()
	{

		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(1, 60);
		std::uniform_real_distribution<> dis2(1, 50);
		int sensorAngleSpacing = int(dis(gen) + 1);
		int sensorOffsetDistance = int(dis2(gen) + 1); 
		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z = sensorOffsetDistance;
		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.a = sensorAngleSpacing;

		//evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.z = this->sensorOffsetDistance;
		//evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.a = this->sensorAngleSpacing;

		std::cout << "sensorOffsetDistance: " + std::to_string(sensorOffsetDistance) << std::endl;
		std::cout << "sensorAngleSpacing: " + std::to_string(sensorAngleSpacing) << std::endl;

	}

	void RandomTrailParams()
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0, 1);
		std::uniform_real_distribution<> disBlend(2, 10);
		float evaporationSpeed = dis(gen);
		float blendSpeed = disBlend(gen);
		//evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.x = evaporationSpeed;
		//evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.y = blendSpeed;

		evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.x = 0.5;
		std::cout << "evaporationSpeed: " + std::to_string(evaporationSpeed) << std::endl;
		std::cout << "blendSpeed: " + std::to_string(blendSpeed) << std::endl;

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

