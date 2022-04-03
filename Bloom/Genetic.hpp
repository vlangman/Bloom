#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include "Neuron.hpp"
#include <cmath>
#include <stack>
#include <list>

class GeneticFactory
{
public:
	GeneticFactory();
	~GeneticFactory();


	std::map < NEURON_TYPES, NeuronPrefab*> GenerateChromosomes(int chromosomeCount);

	NeuronPrefab* GenerateNeuronPrefab(BASE_NEURON_TYPE excludeType, std::map<NEURON_TYPES, NeuronPrefab*>* brainMap, int &count);
	std::map<NEURON_TYPES, NeuronPrefab*> PruneDNA( std::vector<std::vector<NeuronPrefab*>> &vec2);
	void printVec2(std::vector<std::vector<NeuronPrefab*>>& vec2);
	void GenCodex();
	float RandomGen(float min, float max);
	void GenerateNeuronTypes(BASE_NEURON_TYPE excludeType, std::string& chromosome, int& neuronType, int& neuronSubTypeIndex);
	void GenerateTest(std::map<NEURON_TYPES, std::vector<NEURON_TYPES>> test);


	NeuronPrefab* GenerateNeuronTestPrefab(
		BASE_NEURON_TYPE TypeBaseA,
		NEURON_TYPES TypeA,
		std::map<NEURON_TYPES, NeuronPrefab*>* brainMap,
		int& counter
	);

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






	void GeneticFactory::GenerateNeuronTypes(BASE_NEURON_TYPE excludeType, std::string &chromosome, int &neuronType, int &neuronSubTypeIndex) {
		
		//GEN RANDOM NEURON TYPE 1 (N1) OF NOT EXCLUDE TYPE
		//ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum

		//[0-2]
		int enumStart = 0;
		int enumEnd = 3;
		if (SENSOR == excludeType)
			enumStart = 1;
		if (ACTION == excludeType)
			enumEnd = 2;

		neuronType = RandomGen(enumStart, enumEnd);
		std::string neuronTypeGene = std::to_string(neuronType);

		chromosome += neuronTypeGene;

		
		//[0-TypeMax]
		if (INTERNAL == neuronType)
			neuronSubTypeIndex = RandomGen(0, INTERNAL_SIZE);
		else if (SENSOR == neuronType)
			neuronSubTypeIndex = RandomGen(INTERNAL_SIZE, INTERNAL_SIZE + SENSOR_SIZE);
		else
			neuronSubTypeIndex = RandomGen(INTERNAL_SIZE + SENSOR_SIZE, INTERNAL_SIZE + SENSOR_SIZE + ACTIONS_SIZE);

		const char neuronSubTypeGene = neuronCodex[neuronSubTypeIndex];
		chromosome += neuronSubTypeGene;
	}


	NeuronPrefab* GeneticFactory::GenerateNeuronPrefab(BASE_NEURON_TYPE excludeType,std::map<NEURON_TYPES, NeuronPrefab*> *brainMap, int &counter)
	{
		std::string chromosome = "";
		int neuronType = -1;
		int neuronSubTypeIndex = -1;
		GenerateNeuronTypes(excludeType, chromosome, neuronType, neuronSubTypeIndex);

	
		//CHECK IF NEURON EXISTS IN CURRENT BRAINMAP BEFORE CREATING A NEW ONE
		if (brainMap->size() && brainMap->find((NEURON_TYPES)neuronSubTypeIndex) != brainMap->end())
		{
			//copy the old neuron and return it , to start	
			return new NeuronPrefab(brainMap->at((NEURON_TYPES)neuronSubTypeIndex));
			//std::cout << "ALREADY CREATED , FETCHING REF [" + brainMap->at((NEURON_TYPES)neuronSubTypeIndex)->printNeuronType() + "] "  << std::endl;
		}

		counter++;
		NeuronPrefab*newNeuron = new NeuronPrefab((NEURON_TYPES)neuronSubTypeIndex,(BASE_NEURON_TYPE)neuronType, neuronParameterMap[(NEURON_TYPES)neuronSubTypeIndex], counter);
		brainMap->insert({ newNeuron->type, newNeuron });

		return newNeuron;
	
	}

	NeuronPrefab* GeneticFactory::GenerateNeuronTestPrefab(
		BASE_NEURON_TYPE TypeBaseA,
		NEURON_TYPES TypeA, 
		std::map<NEURON_TYPES, NeuronPrefab*>* brainMap,
		int& counter
	)
	{
		std::string chromosome = "";
		

		//CHECK IF NEURON EXISTS IN CURRENT BRAINMAP BEFORE CREATING A NEW ONE
		if (brainMap->find((NEURON_TYPES)TypeA) != brainMap->end())
		{
			//copy the old neuron and return it , to start	
			return new NeuronPrefab(brainMap->at(TypeA));
			//std::cout << "ALREADY CREATED , FETCHING REF [" + brainMap->at((NEURON_TYPES)neuronSubTypeIndex)->printNeuronType() + "] "  << std::endl;
		}

		counter++;
		std::vector<neuronParameter> empty{};
		NeuronPrefab* newNeuron = new NeuronPrefab(TypeA, TypeBaseA, empty, counter);
		brainMap->insert({ newNeuron->type, newNeuron });

		return newNeuron;

	}

	std::pair<int,int> GetRows(std::vector<std::vector<NeuronPrefab*>>& vec2, NEURON_TYPES typeA, NEURON_TYPES typeB) {
		int idxA = -1;
		int idxB = -1;
		for (int x = 0; x < vec2.size(); x++)
		{
			if (vec2[x][0]->type == typeA)
				idxA = x;
			if (vec2[x][0]->type == typeB)
				idxB = x;
		}
		return  { idxA,idxB };

	}

	void AddToArrays(std::vector<std::vector<NeuronPrefab*>>& vec2, NeuronPrefab* A, NeuronPrefab* B)
	{

		auto pair = GetRows(vec2, A->type, B->type);
		int rowOfA = pair.first;
		int rowOfB = pair.second;

		//ONLY A can is connected to B FOR SENSORS AND ACTIONS

		
		if (rowOfA >= 0)
			vec2[rowOfA].push_back(B);
		else
		{
			std::vector<NeuronPrefab*>* test = new std::vector<NeuronPrefab*>{ A };
			vec2.push_back(*test);
			vec2[vec2.size() - 1].push_back(B);
		}

		// ( B IS NOT CONNECTED TO A IF BOTH ARE INTERNALS) otherwise its connected BY 
		//since A will always be and Internal or sensor it will be the one 
		//When checking an internal neuron [A] ... ACTIONS & SENSORS can be thought of as ( Connected by )
		if (INTERNAL == A->baseType && INTERNAL == B->baseType)
			return;

		if (rowOfB >= 0)
			vec2[rowOfB].push_back(A);
		else
		{
			std::vector<NeuronPrefab*>* test = new std::vector<NeuronPrefab*>{ B };
			vec2.push_back(*test);
			vec2[vec2.size() - 1].push_back(A);
		}

	};


	BASE_NEURON_TYPE GetBaseType(NEURON_TYPES in)
	{
		if (in <= 2)
			return INTERNAL;
		else if (in > 2 && in <= 5)
			return SENSOR;
		else
			return ACTION;
	}

	void GeneticFactory::GenerateTest(std::map<NEURON_TYPES, std::vector<NEURON_TYPES>> test) {

		int chromosomeCount = test.size();
		std::map<NEURON_TYPES, NeuronPrefab*> brainMapping;
		std::vector<std::vector<NeuronPrefab*>> grid;
		int counter = -1;

		for (auto neuron : test)
		{
			NeuronPrefab* parent =  GenerateNeuronTestPrefab(GetBaseType(neuron.first),neuron.first,&brainMapping,counter);
			for (auto connectedNeuron : neuron.second)
			{
				NeuronPrefab* child = GenerateNeuronTestPrefab(GetBaseType(connectedNeuron), connectedNeuron, &brainMapping, counter);

				AddToArrays(grid, parent, child);
			}

		}

		PruneDNA(grid);
		
	}

	std::map < NEURON_TYPES, NeuronPrefab*> GeneticFactory::GenerateChromosomes(int chromosomeCount)
	{

		std::map<NEURON_TYPES, NeuronPrefab*> brainMapping;

		// A directed graph using
		// adjacency list representation
		int Indexer = -1; // No. of vertices in graph
		std::vector<std::vector<NeuronPrefab*>> connSquares;
		std::vector<NeuronPrefab*> sensors;

		
		for (int i = 0; i < chromosomeCount; i++)
		{

			//GEN SENSOR OR AN INTERNAL;
			NeuronPrefab*neuronA = this->GenerateNeuronPrefab(ACTION,&brainMapping, Indexer);
			//GEN INTERNAL OR ACTION
			NeuronPrefab*neuronB = this->GenerateNeuronPrefab(SENSOR,&brainMapping, Indexer);

			


			std::string connectionWeight = std::to_string(RandomGen(-4.0, 4.0));
		
			std::string negative = connectionWeight[0] == '-' ? "1": "0";
			if (negative == "1")
				connectionWeight = connectionWeight.substr(1, connectionWeight.length());

			std::string weightGenes = connectionWeight.substr(0, 1) + connectionWeight.substr(2, 2);
			std::string chromosome = neuronA->genes + neuronB->genes + negative + weightGenes;
			
			//std::cout << "CONNECTION WEIGHT: " + negative + weightGenes << std::endl;
			//std::cout << "FULL CHROMOSOME: ["+chromosome+"]" << std::endl;

			std::cout << "CONNECTING [" + neuronA->printNeuronType() + "]" + " TO [" + neuronB->printNeuronType() + "]" << std::endl;
			neuronA->connections.push_back(new connection(neuronB,atof(connectionWeight.c_str())));
			
			//totalVertices += 2;
			
			AddToArrays(connSquares, neuronA, neuronB);
			
			
			if (SENSOR == neuronA->baseType)
			{
				sensors.push_back(neuronA);
				neuronB->sensorInputConnected = true;
				neuronA->sensorInputConnected = true;
			}
			if (ACTION == neuronB->baseType)
			{
				neuronA->connectedToAction = true;
				neuronB->connectedToAction = true;
			}
		
		}

		return PruneDNA(connSquares);


	/*	for (int x = 0; x < connSquares.size(); x++)
		{

			std::cout << connSquares[x][0]->printNeuronType() + " HAS CONNECTIONS: \n";
			for (int y = 1; y < connSquares[x].size(); y++)
			{
				std::cout << "\t " + connSquares[x][y]->printNeuronType() << std::endl;
			}
		}*/

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


	void AddIndirectConnection(NeuronPrefab* parent, NeuronPrefab* child)
	{
		if (SENSOR == parent->baseType)
		{
			child->sensorInputConnected = true;
			parent->connectedToAction |= child->connectedToAction;
		}
		if (ACTION == parent->baseType)
		{
			child->connectedToAction = true;
			parent->sensorInputConnected |= child->sensorInputConnected;
		}
		if (INTERNAL == parent->baseType)
		{
			//if the parent is an internal and the child is sensor or action, mark the parent as connected to sensor or action
			if (SENSOR == child->baseType)
			{
				child->connectedToAction |= parent->connectedToAction;
				parent->sensorInputConnected = true;
			}
			if (ACTION == child->baseType)
			{
				child->sensorInputConnected |= parent->sensorInputConnected;
				parent->connectedToAction = true;

			}
			if (INTERNAL == child->baseType)
			{
				child->sensorInputConnected |= parent->sensorInputConnected;
				parent->connectedToAction |= child->connectedToAction;
			}
		}
	}

	std::map<NEURON_TYPES,NeuronPrefab*>  GeneticFactory::PruneDNA(std::vector<std::vector<NeuronPrefab*>> &vec2)
	{

		for (int x = 0; x < vec2.size() -1; x++)
		{
			NeuronPrefab* parent = vec2[x][0];
			
			
			for (int y = 1; y < vec2[x].size() -1; y++)
			{
			
				NeuronPrefab* child = vec2[x][y];
				AddIndirectConnection(parent, child);

				try
				{
					
					if (parent != NULL &&  vec2[parent->ID].size() > 1)
					{
						for (int count = 1; count < vec2[parent->ID].size() - 1; count++)
						{
							NeuronPrefab* subchild = vec2[parent->ID][count];
							AddIndirectConnection(parent, subchild);
						}
					}

					
					if (child->ID < vec2.size() && vec2[child->ID].size() > 1)
					{
						for (int count = 1; count < vec2[child->ID].size() - 1; count++)
						{
							NeuronPrefab* subchild = vec2[child->ID][count];
							AddIndirectConnection(child, subchild);
						}

					}
				}
				catch (const std::exception& e)
				{
					printVec2(vec2);
				}

				


			}
				
		}
		printVec2(vec2);
	

		std::map<NEURON_TYPES,NeuronPrefab*> program{};
		for (auto neuronConns : vec2)
		{
			for (auto neuron : neuronConns)
			{
				if (SENSOR == neuron->baseType && neuron->connectedToAction)
					program[neuron->type] = neuron;
				else if (ACTION == neuron->baseType && neuron->sensorInputConnected)
					program[neuron->type] = neuron;
				else if (neuron->connectedToAction && neuron->sensorInputConnected)
					program[neuron->type] = neuron;

			}

		}
		std::cout << "PROGRAM:" << std::endl;
		for (auto n : program)
			std::cout << n.second->printNeuronType() << std::endl;
		
		return program;
	}


	void GeneticFactory::printVec2(std::vector<std::vector<NeuronPrefab*>> &vec2)
	{
		for (int x = 0; x < vec2.size(); x++)
		{
			std::cout << (vec2[x][0]->printNeuronType()) + "\n";
			for (int y = 1; y < vec2[x].size(); y++)
			{
				std::string keep = (vec2[x][y]->connectedToAction && vec2[x][y]->sensorInputConnected) ? "[Y]" : "[" + std::to_string((int)vec2[x][y]->sensorInputConnected) + std::to_string((int)vec2[x][y]->connectedToAction) + "]";
				std::cout << "\t"+(vec2[x][y]->printNeuronType())+ keep<<std::endl;
			}
			std::cout << std::endl;

		}
	}



	float GeneticFactory::RandomGen(float min, float max)
	{
		//GEN RANDOM NEURON TYPE 1 (N1) ASSIGN A CHARACTER WITH INDEX IN NEURON_TYPE enum
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(min, max);
		return dist(gen);
	}