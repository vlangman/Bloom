
#include <string>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include "NeuronParameter.hpp"
#include "Utils.hpp"


//forward declarations

struct connection;

struct NeuronPrefab {

	NEURON_TYPES type;
	BASE_NEURON_TYPE baseType;
	
	std::string genes = "";

	int maxParameters = 4;
	std::vector<neuronParameter> parameters;
	std::vector<connection*> connections;

	int ID = -1;

	bool sensorInputConnected = false;
	bool connectedToAction = false;
	
	NeuronPrefab(){}
	
	NeuronPrefab(NeuronPrefab* n) {
		this->type = n->type;
		this->baseType = n->baseType;
		this->parameters = n->parameters;
		this->maxParameters = n->maxParameters;
		this->ID = n->ID;
		this->connectedToAction = n->connectedToAction;
		this->sensorInputConnected = n->sensorInputConnected;
	}

	NeuronPrefab(NEURON_TYPES type, BASE_NEURON_TYPE base, std::vector<neuronParameter> params, int id) {
		this->ID = id;
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

	std::string printNeuronType()
	{
		std::string ret = "";

		if (SENSOR == this->baseType)
		{
			ret = "\x1B[32m" + neuronTypes[this->type] + "\033[0m";
		}
		else if (INTERNAL == this->baseType) {
			ret = "\x1B[33m" + neuronTypes[this->type] + "\033[0m";
		}
		else
			ret = "\x1B[91m" + neuronTypes[this->type] + "\033[0m";

		return ret;
	}

	std::string GenesToBinary()
	{
		std::string binary = "";
		for (char c : this->genes)
		{
			binary += Utils::hex_char_to_bin(c);
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
		//BinToHex(GenesToBinary());
	
	}

	NeuronPrefab* GetRef() {
		return this;
	}

	void DebugParameters()
	{
		//std::cout << "CREATING NEW Neuron of type : " + this->printNeuronType() << std::endl;
		/*std::cout << "Parameters" << std::endl;

		for (neuronParameter neuron : this->parameters)
		{
			std::cout << "\t" + parameterTypes[(PARAMETER_TYPE)neuron.parameterType] + " value : " + std::to_string(neuron.value) << std::endl;
		}*/
	}




};




struct connection {
	bool isValidConnection = false;

	connection(NeuronPrefab* n, float weight) {

		this->neuron = n;
		this->connectionWeight = weight;
	}
	NeuronPrefab* neuron;
	float connectionWeight;
};