#pragma once
#include "Neuron.hpp"


struct Forward_Line_Sensor : virtual public Neuron {
	//parameters
	Forward_Line_Sensor() :
		Neuron(FORWARD_LINE_SENSOR, std::vector<neuronParameter>({
				neuronParameter(LINE_SENSOR_ANGLES,0,1),
				neuronParameter(LINE_SENSOR_LENGTH,0,51)
			})
		) {
		std::cout << "CREATED Forward_Line_Sensor" << std::endl;
	}
};