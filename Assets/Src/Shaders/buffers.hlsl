#include "Organisms/organism.hlsl"
#include "Neurons/neuron.hlsl"
#include "Pheromones/pheromone.hlsl"


#ifndef __BUFFERS
#define __BUFFERS

uniform AppendStructuredBuffer<uint> organismFiltered; //Filtered index
uniform RWStructuredBuffer<Organism> organismBuffer : register(u1); //has to be same name with rendering shader
uniform RWStructuredBuffer<Neuron> neuronBuffer : register(u2);
uniform RWStructuredBuffer<Pheromone> pheromoneBuffer : register(u3);

uniform RWTexture2D<float4> organismTexture;
uniform RWTexture2D<float4> pheromoneTexture;

#endif