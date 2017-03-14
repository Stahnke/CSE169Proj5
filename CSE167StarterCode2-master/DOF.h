#ifndef _DOF_H
#define _DOF_H_

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

class DOF
{
private:
	float value, min, max;

public:
	DOF();
	~DOF();

	void SetValue(float value);
	void SetMinMax(float min, float max);
	float GetValue();
	float GetMin();
	float GetMax();

};

#endif
