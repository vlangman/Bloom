#pragma once
#include <map>
#include "NeuronTypes.hpp"



enum PARAMETER_TYPE
{
	//SENSORS
	LINE_SENSOR_ANGLES,
	LINE_SENSOR_LENGTH,

	//ACTIONS
	MOVE_SPEED,
	TURN_SPEED,
};



struct neuronParameter {

	PARAMETER_TYPE parameterType;
	float value = -1;

	float min = 0;
	float max = 0;

	neuronParameter(PARAMETER_TYPE parameterType, float m, float ma) {
		this->min = m;
		this->max = ma;
		this->parameterType = parameterType;
	}

	void generateValue() {
		//GEN RANDOM NEURON TYPE 1 (N1) ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> valueDist(this->min, this->max);
		this->value = int(valueDist(gen));
		//std::cout << "GEN PARAM : " + std::to_string(this->value) << std::endl;

	}
};



//MAPING PARAMETER_TYPE FOR DEBUGGING WITH STRINGS
std::map<PARAMETER_TYPE, std::string> parameterTypes({
	//SENSORS
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_ANGLES,"LINE_SENSOR_ANGLES"),
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_LENGTH,"LINE_SENSOR_LENGTH"),

	//ACTIONS
	std::pair<PARAMETER_TYPE,std::string>(MOVE_SPEED,"MOVE_SPEED"),
	std::pair<PARAMETER_TYPE,std::string>(TURN_SPEED,"TURN_SPEED")
});


//this is a map of all neuron types to their generated parameter types
std::map< NEURON_TYPES, std::vector<neuronParameter> > neuronParameterMap = {

#pragma region SENSORS 

	//RIGHT_LINE_SENSOR
	std::pair<NEURON_TYPES, std::vector<neuronParameter>>(RIGHT_LINE_SENSOR, std::vector<neuronParameter>({
		neuronParameter(LINE_SENSOR_ANGLES,0,180),
		neuronParameter(LINE_SENSOR_LENGTH,0,51),
	})),
	//LEFT_LINE_SENSOR
	std::pair<NEURON_TYPES, std::vector<neuronParameter>>(LEFT_LINE_SENSOR, std::vector<neuronParameter>({
		neuronParameter(LINE_SENSOR_ANGLES,0,180),
		neuronParameter(LINE_SENSOR_LENGTH,0,51),
	})),

	//FORWARD_LINE_SENSOR
	std::pair<NEURON_TYPES, std::vector<neuronParameter>>(FORWARD_LINE_SENSOR, std::vector<neuronParameter>({
		neuronParameter(LINE_SENSOR_LENGTH,0,51),
	})),

	#pragma endregion

	#pragma region ACTIONS


	//MOVE_SPEED
	std::pair<NEURON_TYPES, std::vector<neuronParameter>>(MOVE_FORWARD, std::vector<neuronParameter>({
			neuronParameter(MOVE_SPEED,0,51),
	})),
	//TURN_TARGET_DIRECTION_RIGHT
	std::pair<NEURON_TYPES, std::vector<neuronParameter>>(TURN_TARGET_DIRECTION_RIGHT, std::vector<neuronParameter>({
			neuronParameter(TURN_SPEED,0,180),
	})),
	//TURN_TARGET_DIRECTION_LEFT
	std::pair<NEURON_TYPES, std::vector<neuronParameter>>(TURN_TARGET_DIRECTION_LEFT, std::vector<neuronParameter>({
			neuronParameter(TURN_SPEED,0,180),
	}))
}
	#pragma endregion