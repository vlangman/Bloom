Shader "Simulation"
{
	Properties 
	{
		// _pSize("NeuronCount",Range(1,5)) = 2 //seems only work for Metal :(
		//number of neurons per organism
		_NeuronCount("NeuronCount",Range(1,10)) = 1
		// //NEURONS
		// //number of connections per neuron
		_ConnectionCount("ConnectionCount",Range(1,10)) = 1

		// //number of parameters per neuron
		_ParameterCount("ParameterCount",int) = 4

	}

	SubShader 
	{
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
			
			StructuredBuffer<Organism> organismBuffer;  //has to be same name with compute shader
			StructuredBuffer<uint> organismFiltered;  //has to be same name with compute shader

			struct v2f
			{
				float4 color : COLOR;
				float4 position : SV_POSITION;
			};
			
			v2f vert (uint inst : SV_InstanceID)
			{
				v2f o;
				// uint id = organismFiltered[inst];

				//normalize world space with resolution
				float2 screenSpace = (organismBuffer[inst].position)/512.0;
				float4 pos = float4( screenSpace.x*2.0-1.0, screenSpace.y*2.0-1.0,1,1);
				
				o.position = pos;
				//alive 
				if(organismBuffer[inst].alive)
				{
					o.color = organismBuffer[inst].color;
				}
				else{
					o.color = float4(1,screenSpace.y,screenSpace.x,0);
				}	
				
				return o;
			}
			
			float4 frag (v2f IN) : SV_Target
			{
				return IN.color;
			}
			
			ENDCG
			
		}
	}
}