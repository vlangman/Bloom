#pragma once
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdlib.h>


const int INTERNAL_SIZE = 3;
const int SENSOR_SIZE = 3;
const int ACTIONS_SIZE = 3;

enum BASE_NEURON_TYPE
{
	SENSOR,
	INTERNAL,
	ACTION
};

enum NEURON_TYPES {
	//internal
	INTERNAL_0,
	INTERNAL_1,
	INTERNAL_2,

	//sensors
	FORWARD_LINE_SENSOR,
	LEFT_LINE_SENSOR,
	RIGHT_LINE_SENSOR,

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

	//ACTIONS
	MOVE_SPEED,
	TURN_SPEED,
};


//MAPING BASE_NEURON_TYPES FOR DEBUGGING WITH STRINGS
std::map<BASE_NEURON_TYPE, std::string> baseNeuronTypes({
	std::pair<BASE_NEURON_TYPE,std::string>(SENSOR,"SENSOR"),
	std::pair<BASE_NEURON_TYPE,std::string>(INTERNAL,"INTERNAL"),
	std::pair<BASE_NEURON_TYPE,std::string>(ACTION,"ACTION")
});

//MAPING NEURON_TYPES FOR DEBUGGING WITH STRINGS
std::map<NEURON_TYPES, std::string> neuronTypes({
	std::pair<NEURON_TYPES,std::string>(INTERNAL_0,"INTERNAL_0"),
	std::pair<NEURON_TYPES,std::string>(INTERNAL_1,"INTERNAL_1"),
	std::pair<NEURON_TYPES,std::string>(INTERNAL_2,"INTERNAL_2"),

	//SENSORS
	std::pair<NEURON_TYPES,std::string>(FORWARD_LINE_SENSOR,"FORWARD_LINE_SENSOR"),
	std::pair<NEURON_TYPES,std::string>(LEFT_LINE_SENSOR,"LEFT_LINE_SENSOR"),
	std::pair<NEURON_TYPES,std::string>(RIGHT_LINE_SENSOR,"RIGHT_LINE_SENSOR"),


	//ACTIONS
	std::pair<NEURON_TYPES,std::string>(MOVE_FORWARD,"MOVE_FORWARD"),
	std::pair<NEURON_TYPES,std::string>(TURN_TARGET_DIRECTION_RIGHT,"TURN_TARGET_DIRECTION_RIGHT"),
	std::pair<NEURON_TYPES,std::string>(TURN_TARGET_DIRECTION_LEFT,"TURN_TARGET_DIRECTION_LEFT"),

});


//MAPING PARAMETER_TYPE FOR DEBUGGING WITH STRINGS
std::map<PARAMETER_TYPE, std::string> parameterTypes({
	//SENSORS
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_ANGLES,"LINE_SENSOR_ANGLES"),
	std::pair<PARAMETER_TYPE,std::string>(LINE_SENSOR_LENGTH,"LINE_SENSOR_LENGTH"),

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

#pragma endregion

};




void createMap(std::map<std::string, char>* um)
{
	(*um)["0000"] = '0';
	(*um)["0001"] = '1';
	(*um)["0010"] = '2';
	(*um)["0011"] = '3';
	(*um)["0100"] = '4';
	(*um)["0101"] = '5';
	(*um)["0110"] = '6';
	(*um)["0111"] = '7';
	(*um)["1000"] = '8';
	(*um)["1001"] = '9';
	(*um)["1010"] = 'A';
	(*um)["1011"] = 'B';
	(*um)["1100"] = 'C';
	(*um)["1101"] = 'D';
	(*um)["1110"] = 'E';
	(*um)["1111"] = 'F';
}

const char* hex_char_to_bin(char c)
{
	// TODO handle default / error
	switch (toupper(c))
	{
	case '0': return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': return "1010";
	case 'B': return "1011";
	case 'C': return "1100";
	case 'D': return "1101";
	case 'E': return "1110";
	case 'F': return "1111";
	}
}

// function to find hexadecimal
// equivalent of binary
std::string BinToHex(std::string bin)
{
	int l = bin.size();
	for (int i = 1; i <= (4 - l % 4) % 4; i++)
		bin = '0' + bin;

	std::map<std::string, char> bin_hex_map;
	createMap(&bin_hex_map);

	int i = 0;
	std::string hex = "";

	while (1)
	{
		hex += bin_hex_map[bin.substr(i, 4)];
		i += 4;
		if (i == bin.size())
			break;
	}

	//std::cout << "BINARY TO HEX [" + hex + "]" << std::endl;
	// required hexadecimal number
	return hex;
}

struct connection;

struct Neuron {

	NEURON_TYPES type;
	BASE_NEURON_TYPE baseType;
	
	std::string spacerChar = "x0";
	std::string genes = "";

	std::vector<neuronParameter> parameters;
	int maxParameters = 4;

	float value; 
	
	int connectedSensors = 0;
	std::vector<connection*> connections;

	Neuron(){}

	Neuron(NEURON_TYPES type, BASE_NEURON_TYPE base, std::vector<neuronParameter> params) {
	
		this->type = type;
		this->baseType = base;

		//populate the random parameters in the list of params
		for (neuronParameter param : params)
		{
			neuronParameter copiedParam = neuronParameter(param.parameterType, param.min, param.max);
			copiedParam.generateValue();
			this->parameters.push_back(copiedParam);
		}
		WriteGenes();

		DebugParameters();
	}

	

	std::string GenesToBinary()
	{
		std::string binary = "";
		for (char c : this->genes)
		{
			binary += hex_char_to_bin(c);
		}
		//std::cout << "BINARY VERSION [" + binary + "]" << std::endl;
		return binary;
	}

	void WriteGenes() {
		this->genes += std::to_string(this->type);
		for (auto gene : this->parameters)
		{
			//ensures no parameter values > 255 (1 byte)
			char buffer[3];
			_itoa_s(gene.value, buffer, 16);

			//pad values so params have 2 vals;
			if (gene.value < 16)
				this->genes += '0';

			this->genes += buffer;
		}

		//can store NeuronType && 3 params in 16 byte 

		//pad 4 - paramcount *2 bits
		/*for (int i = 0; i < (4 - this->parameters.size()) ; i++)
			this->genes += this->spacerChar;*/
		//std::cout << "Neuron Genes: [" + this->genes + "]" << std::endl;
		BinToHex(GenesToBinary());
	
	}

	void DebugParameters()
	{
		std::cout << "CREATING NEW Neuron of type : " + neuronTypes[(NEURON_TYPES)this->type] << std::endl;
		/*std::cout << "Parameters" << std::endl;

		for (neuronParameter neuron : this->parameters)
		{
			std::cout << "\t" + parameterTypes[(PARAMETER_TYPE)neuron.parameterType] + " value : " + std::to_string(neuron.value) << std::endl;
		}*/
	}

};

struct connection {
	connection(Neuron * p, float weight) {
		this->neuron = p;

		this->connectionWeight = weight;
	}
	Neuron * neuron;
	float connectionWeight;
};