#include "DOF.h"

DOF::DOF() {
	//init
	min = std::numeric_limits<int>::max() * -1;
	max = std::numeric_limits<int>::max();
	value = 0;
}

DOF::~DOF() {

}

void DOF::SetValue(float value) {
	this->value = value;
	
	if (this->value < min)
		this->value = min;
	
	if (this->value > max)
		this->value = max;
}

void DOF::SetMinMax(float min, float max) {
	this->min = min;
	this->max = max;
}

float DOF::GetValue() {
	return value;
}

float DOF::GetMin() {
	return min;
}

float DOF::GetMax() {
	return max;
}

