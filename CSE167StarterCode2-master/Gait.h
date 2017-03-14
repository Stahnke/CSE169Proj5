#ifndef _GAIT_H
#define _GAIT_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//from proj2
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>

#include "Step.h"

using namespace std;

class Gait
{
private:
	float maxTime;
	float curTime;

	vector<Step*> steps;

public:
	Gait(float dutyFactor, vector<float> triggers);
	~Gait();

	void Update(float stepSize);
	vector<float> GetCurTimes();
	vector<float> GetDutyFactors();
};

#endif
