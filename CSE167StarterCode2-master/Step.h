#ifndef _STEP_H
#define _STEP_H_

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

using namespace std;

class Step
{
private:

	float curTime;
	float stepTrigger;
	float maxTime;
	float dutyFactor;

public:
	Step(float stepTrigger, float maxTime, float dutyFactor);
	~Step();

	void Update(float curTime);
	float GetCurTime();
	float GetDutyFactor();
};

#endif
