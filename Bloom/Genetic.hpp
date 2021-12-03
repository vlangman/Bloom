#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include "Neuron.hpp"
#include "Sensors.hpp"


	enum BASE_NEURON_TYPE
	{
		INTERNAL,
		SENSOR,
		ACTION
	};


	std::map<BASE_NEURON_TYPE, std::string> baseNeurons({ 
		std::pair<BASE_NEURON_TYPE,std::string>(SENSOR,"SENSOR"),
		std::pair<BASE_NEURON_TYPE,std::string>(INTERNAL,"INTERNAL"),
		std::pair<BASE_NEURON_TYPE,std::string>(ACTION,"ACTION")
	});


	
	
	class GeneticFactory
	{
	public:
		GeneticFactory();
		~GeneticFactory();

		void GenerateNeuron();
		void GenCodex();
		int RandomGen(float min, float max);
	private:
		std::vector<char> neuronCodex;
		int chromosomeCount = 1;
		std::vector<std::string> genome;
	};

	GeneticFactory::GeneticFactory()
	{
		int chromosomeCount = 1;
		GenCodex();

		
	}

	GeneticFactory::~GeneticFactory()
	{
	}



	void GeneticFactory::GenerateNeuron()
	{

		std::string chromosome = "";

		//GEN RANDOM NEURON TYPE 1 (N1) ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum
		
		//[0-2]
		int neuronType = RandomGen(0, 3);
		std::string neuronTypeGene = std::to_string(neuronType);
		std::cout <<  "TYPE: " + baseNeurons[(BASE_NEURON_TYPE)neuronType] << std::endl;
		chromosome += neuronTypeGene;

		// SELECT N1 NEURON_NAME RANDOMLY
		int neuronSubTypeIndex = -1;
		//[0-TypeMax]
		
		if (INTERNAL == neuronType)
			neuronSubTypeIndex = RandomGen(0, INTERNAL_SIZE);
		else if (SENSOR == neuronType)
			neuronSubTypeIndex = RandomGen(INTERNAL_SIZE, INTERNAL_SIZE+ SENSOR_SIZE);
		else 
			neuronSubTypeIndex = RandomGen(INTERNAL_SIZE+ SENSOR_SIZE, INTERNAL_SIZE + SENSOR_SIZE + ACTIONS_SIZE);
		
		const char neuronSubTypeGene = neuronCodex[neuronSubTypeIndex];
		chromosome += neuronSubTypeGene;
		

		Neuron *newNeuron = new Neuron((NEURON_TYPES)neuronSubTypeIndex, neuronParameterMap[(NEURON_TYPES)neuronSubTypeIndex]);


		//GEN N1 ConnectionWeight
	
	}


	void GeneticFactory::GenCodex() {
		int countN = 0;
		std::vector<char> neuronCodex;
		char buffer;
		while (countN++ < INTERNAL_SIZE + SENSOR_SIZE + ACTIONS_SIZE)
		{
			neuronCodex.push_back((48 + countN) + '0');
		}
		this->neuronCodex = neuronCodex;
		
	}

	int GeneticFactory::RandomGen(float min, float max)
	{
		//GEN RANDOM NEURON TYPE 1 (N1) ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(min, max);
		return dist(gen);
	}