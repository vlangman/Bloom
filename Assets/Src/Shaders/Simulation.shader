Shader "Simulation"
{
	Properties 
	{
		// _pSize("NeuronCount",Range(1,5)) = 2 //seems only work for Metal :(
		// //number of neurons per organism
		// _NeuronCount("NeuronCount",Range(1,10)) = 1
		// // //NEURONS
		// // //number of connections per neuron
		// _ConnectionCount("ConnectionCount",Range(1,10)) = 1

		// // //number of parameters per neuron
		_resolution("_resolution",Vector) = (512,512.0,0)

	}

	SubShader 
	{
		Tags {"Queue" = "Transparent" "RenderType"="Transparent" }
		//// 1st pass render pheromones
		Pass 
		{
			Blend SrcAlpha OneMinusSrcAlpha

			CGPROGRAM
			#pragma target 5.0
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"
			#include "globals.hlsl"
			#include "Pheromones/pheromone.hlsl"

			// /cant include buffers.hlsl here they are different StructuredBuffer types
			StructuredBuffer<Pheromone> pheromoneBuffer;
			uniform float4 _resolution;

			struct v2f
			{
				float4 colour : colour;
				float4 position : SV_POSITION;
			};
			
			v2f vert (uint index : SV_InstanceID)
			{
				v2f o;

				float2 uv = float2(index/_resolution.x,index%_resolution.y)/_resolution.x;
				// uint index = Index(uv*_resolution.x);
				Pheromone pheromone = pheromoneBuffer[index];
				
				float4 pos = float4( uv.x*2.0-1.0, uv.y*2.0-1.0,1,1);
				o.position = pos;
				o.colour = pheromone.colour;
				return o;
			}
			
			float4 frag (v2f IN) : SV_Target
			{
				return IN.colour;
			}
			
			ENDCG
			
		}
		// 2nd pass render organisms
		Pass 
		{
			Blend SrcAlpha OneMinusSrcAlpha

			CGPROGRAM
			#pragma target 5.0
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"
			
			#include "globals.hlsl"
			#include "Organisms/organism.hlsl"
			#include "Pheromones/pheromone.hlsl"

			// /cant include buffers.hlsl here they are different StructuredBuffer types
			StructuredBuffer<Organism> organismBuffer;
			StructuredBuffer<uint> organismFiltered;
			StructuredBuffer<Pheromone> pheromoneBuffer;
			uniform float4 _resolution;
			struct v2f
			{
				float4 colour : colour;
				float4 position : SV_POSITION;
			};
			
			v2f vert (uint inst : SV_InstanceID )
			{
				v2f o;
				
				Organism organism = organismBuffer[inst];
				float2 worldSpace = organism.position/_resolution.x;
				float4 pos = float4( worldSpace.x*2.0-1.0, worldSpace.y*2.0-1.0,1,1);
				o.position = pos;

				if(organism.alive == 1)
				{
					o.colour = organism.colour;
				}
				else
				{
					// o.colour = pheromoneBuffer[inst].colour;
					o.colour = float4(0,0,0,0);
				}
				
				
				
				return o;
			}
			
			float4 frag (v2f IN) : SV_Target
			{
				return IN.colour;
			}
			
			ENDCG
			
		}

	}
}