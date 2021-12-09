#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include "Neuron.hpp"
#include "Sensors.hpp"
#include <cmath>



class GeneticFactory
{
public:
	GeneticFactory();
	~GeneticFactory();

	void GenerateChromosomes(int chromosomeCount);

	Neuron* GenerateNeuron(BASE_NEURON_TYPE excludeType, std::map<NEURON_TYPES, Neuron*>* brainMap);
	std::map<NEURON_TYPES, Neuron*>  PruneDNA(std::vector<Neuron>* neurons);
	void GenCodex();
	float RandomGen(float min, float max);
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









	Neuron * GeneticFactory::GenerateNeuron(BASE_NEURON_TYPE excludeType,std::map<NEURON_TYPES,Neuron*> *brainMap)
	{

		std::string chromosome = "";

		//GEN RANDOM NEURON TYPE 1 (N1) OF NOT EXCLUDE TYPE
		//ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum
		
		//[0-2]
		int enumStart = 0;
		int enumEnd = 3;
		if (SENSOR == excludeType)
			enumStart = 1;
		if (ACTION == excludeType)
			enumEnd = 2;

		int neuronType = RandomGen(enumStart, enumEnd);
		std::string neuronTypeGene = std::to_string(neuronType);

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
		
		//CHECK IF NEURON EXISTS IN CURRENT BRAINMAP BEFORE CREATING A NEW ONE
		if (brainMap->size() && brainMap->find((NEURON_TYPES)neuronSubTypeIndex) != brainMap->end())
		{
			//std::cout << "ALREADY CREATED , FETCHING REF [" + brainMap->at((NEURON_TYPES)neuronSubTypeIndex)->printNeuronType() + "] "  << std::endl;
			return brainMap->at((NEURON_TYPES)neuronSubTypeIndex);
		}


		Neuron *newNeuron = new Neuron((NEURON_TYPES)neuronSubTypeIndex,(BASE_NEURON_TYPE)neuronType, neuronParameterMap[(NEURON_TYPES)neuronSubTypeIndex]);
		brainMap->insert({ newNeuron->type, newNeuron });

		return newNeuron;
		//GEN N1 ConnectionWeight
	
	}


	void GeneticFactory::GenerateChromosomes(int chromosomeCount)
	{

		std::map<NEURON_TYPES, Neuron*> brainMapping;
		std::vector<Neuron> neuronTree;
		for (int i = 0; i < chromosomeCount; i++)
		{

			//GEN SENSOR OR AN INTERNAL;
			Neuron *neuronA = this->GenerateNeuron(ACTION,&brainMapping);
			//GEN INTERNAL OR ACTION
			Neuron *neuronB = this->GenerateNeuron(SENSOR,&brainMapping);


			std::string connectionWeight = std::to_string(RandomGen(-4.0, 4.0));
		
			std::string negative = connectionWeight[0] == '-' ? "1": "0";
			if (negative == "1")
				connectionWeight = connectionWeight.substr(1, connectionWeight.length());
			std::string weightGenes = connectionWeight.substr(0, 1) + connectionWeight.substr(2, 2);


			std::string chromosome = neuronA->genes + neuronB->genes + negative + weightGenes;
			
			//std::cout << "CONNECTION WEIGHT: " + negative + weightGenes << std::endl;
			//std::cout << "FULL CHROMOSOME: ["+chromosome+"]" << std::endl;

			neuronA->connections.push_back(new connection(neuronB,atof(connectionWeight.c_str())));
			std::cout << "CONNECTING [" + neuronA->printNeuronType() +"]" + " TO [" + neuronB->printNeuronType() + "]"  << std::endl;
			

			if (SENSOR == neuronA->baseType)
			{
				std::cout << "INCREMENTING  CONNECTED SENSORS FOR " + neuronB->printNeuronType() << std::endl;
				neuronB->connectedSensors++;
				neuronTree.push_back(*neuronA);
			}
			if (INTERNAL == neuronA->baseType)
			{
				neuronTree.push_back(*neuronA);
			}
	
		}

		std::map<NEURON_TYPES, Neuron*> program = PruneDNA(&neuronTree);

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

	std::map<NEURON_TYPES, Neuron*>  GeneticFactory::PruneDNA(std::vector<Neuron> * sensorNeurons)
	{
		std::map<NEURON_TYPES, Neuron*> selected;

		std::cout << "CHECKING PRUNE :: COUNT " + std::to_string(sensorNeurons->size()) << std::endl <<std::endl;

		std::map<NEURON_TYPES, bool> visited;

		for(auto n = sensorNeurons->begin(); n != sensorNeurons->end(); n++)
		{
			std::cout << "CHECKING NEURON OF TYPE [" + n->printNeuronType() + "]" << std::endl;
			visited[n->type] = true;
			bool leadsToAction = false;
			//CREATE A STACK FOR ALL POSSIBLE CONNECTIONS
			std::vector<connection*> stack = std::vector<connection*>(n->connections);
			//LOOP THROUGH THE STACK TO FIND AN ACTION CONN ELSE ADD THE CONNECTIONS IF NOT SELF REFERENCE
			for(int i = 0; i < stack.size(); i++)
			{
				connection* conn = stack[i];
				if (visited[conn->neuron->type])
					continue;
				std::cout << "\t" << "CHECKING ["+ n->printNeuronType() +"] CONNECTION TYPE: "  + conn->neuron->printNeuronType() << std::endl;

				if (ACTION == conn->neuron->baseType)
				{
					leadsToAction = true;
					if(SENSOR == n->type || (INTERNAL == n->type && n->connectedSensors > 0))
						std::cout << "\t"+ n->printNeuronType() + "---> [" + conn->neuron->printNeuronType() + "] ADD TO PROGRAM"<< std::endl;
					break;
				}
				else
				{
					for (connection* subCon : conn->neuron->connections)
					{
						if (subCon != conn ) {
							std::cout << "\t"+ conn->neuron->printNeuronType() +" IDENTIFIED POSSIBLE CONNECTION ---> " + subCon->neuron->printNeuronType() << std::endl;
							stack.push_back(subCon);
						}
					}
				}
			}

			stack.clear();
			visited.clear();
	
			if (INTERNAL == n->baseType)
			{
				if (n->connectedSensors > 0 && leadsToAction && selected.find(n->type) == selected.end())
					selected.insert({ n->type, n->GetRef() });
				else
				{
					std::string debug = leadsToAction ? "HAS NO [SENSORS] CONNECTED" : "CONNECTS TO NO [ACTIONS]";
					std::cout << "\t"+ n->printNeuronType() + " " + debug  << std::endl;
				}
			}
			else if (SENSOR == n->baseType)
			{
				if (leadsToAction && selected.find(n->type) == selected.end())
				{
					std::cout << "ADDING [" + n->printNeuronType()+"] TO PROGRAM " << std::endl;
					selected.insert({ n->type, n->GetRef()});
					
				}
				else
					std::cout << n->printNeuronType() + " CONNECTS TO NO ACTION OUTPUT "  << std::endl;

			}

		}

		std::cout << "RESULTANT ARRAY PROGRAM : " << std::endl;
		for (auto itt = selected.begin() ; itt != selected.end(); itt++)
		{
			std::cout << "\t" +  itt->second->printNeuronType() << std::endl;
		}


		return selected;
	}



	float GeneticFactory::RandomGen(float min, float max)
	{
		//GEN RANDOM NEURON TYPE 1 (N1) ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(min, max);
		return dist(gen);
	}