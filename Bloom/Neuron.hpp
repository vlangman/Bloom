#pragma once
#include <vector>
#include <map>
#include <string>


const int INTERNAL_SIZE = 3;
const int SENSOR_SIZE = 2;
const int ACTIONS_SIZE = 3;

enum NEURON_TYPES {
	//internal
	INTERNAL_0,
	INTERNAL_1,
	INTERNAL_2,

	//sensors
	FORWARD_LINE_SENSOR,
	SIDE_LINE_SENSOR,

	//actions
	MOVE_FORWARD,
	TURN_TARGET_DIRECTION_RIGHT,
	TURN_TARGET_DIRECTION_LEFT,
};

enum PARAMETER_TYPE
{
	//SENSORS
	LINE_SENSOR_ANGLES,
	LINE_SENSOR_LENGTH,
	LINE_SENSOR_COUNT,

	//ACTIONS
	MOVE_SPEED,
	TURN_SPEED,
};

//MAPING NEURON_TYPES FOR DEBUGGING WITH STRINGS
std::map<NEURON_TYPES, std::string> neuronTypes({
	std::pair<NEURON_TYPES,std::string>(INTERNAL_0,"INTERNAL_0"),
	std::pair<NEURON_TYPES,std::string>(INTERNAL_1,"INTERNAL_1"),
	std::pair<NEURON_TYPES,std::string>(INTERNAL_2,"INTERNAL_2"),

	//SENSORS
	std::pair<NEURON_TYPES,std::string>(FORWARD_LINE_SENSOR,"FORWARD_LINE_SENSOR"),
	std::pair<NEURON_TYPES,std::string>(SIDE_LINE_SENSOR,"SIDE_LINE_SENSOR"),
	std::pair<NEURON_TYPES,std::string>(MOVE_FORWARD,"MOVE_FORWARD"),
	//ACTIONS
	std::pair<NEURON_TYPES,std::string>(TURN_TARGET_DIRECTION_RIGHT,"TURN_TARGET_DIRECTION_RIGHT"),
	std::pair<NEURON_TYPES,std::string>(TURN_TARGET_DIRECTION_LEFT,"TURN_TARGET_DIRECTION_LEFT"),

});


//MAPING PARAMETER_TYPE FOR DEBUGGING WITH STRINGS
std::map<PARAMETER_TYPE, std::string> parameterTypes({
	//SENSORS
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_ANGLES,"LINE_SENSOR_ANGLES"),
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_LENGTH,"LINE_SENSOR_LENGTH"),
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_COUNT,"LINE_SENSOR_COUNT"),

	//ACTIONS
	std::pair<PARAMETER_TYPE,std::string>(MOVE_SPEED,"MOVE_SPEED"),
	std::pair<PARAMETER_TYPE,std::string>(TURN_SPEED,"TURN_SPEED")
});



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
		std::uniform_real_distribution<> valueDist(this->min,this->max);
		this->value = int(valueDist(gen));
		//std::cout << "GEN PARAM : " + std::to_string(this->value) << std::endl;

	}
};


//this is a map of all neuron types to their generated parameter types
std::map< NEURON_TYPES, std::vector<neuronParameter> > neuronParameterMap = {

#pragma region SENSORS 

//SIDE_LINE_SENSOR
std::pair<NEURON_TYPES, std::vector<neuronParameter>>(SIDE_LINE_SENSOR, std::vector<neuronParameter>({
	neuronParameter(LINE_SENSOR_ANGLES,0,86),
	neuronParameter(LINE_SENSOR_LENGTH,0,51),
	neuronParameter(LINE_SENSOR_COUNT,1,4),
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

#pragma endregion

};



struct Neuron {

	NEURON_TYPES type;
	int maxParameters = 4;
	char spacerChar = 'z';
	std::vector<neuronParameter> parameters;

	float weight;

	std::vector<Neuron> inBound;
	std::vector<Neuron> outBound;

	Neuron(NEURON_TYPES type, std::vector<neuronParameter>  params) {
	
		this->type = type;

		//populate the random parameters in the list of params
		for (neuronParameter param : params)
		{
			neuronParameter copiedParam = neuronParameter(param.parameterType, param.min, param.max);
			copiedParam.generateValue();
			this->parameters.push_back(copiedParam);
		}


		DebugParameters();
	}

	void DebugParameters()
	{
		std::cout << "Neuron of type : " + neuronTypes[(NEURON_TYPES)this->type] << std::endl;
		std::cout << "Parameters" << std::endl;

		for (neuronParameter neuron : this->parameters)
		{
			std::cout << "\t" + parameterTypes[(PARAMETER_TYPE)neuron.parameterType] + " value : " + std::to_string(neuron.value) << std::endl;
		}
	}

};