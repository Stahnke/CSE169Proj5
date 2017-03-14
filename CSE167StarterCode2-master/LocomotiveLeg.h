#ifndef _LOCOMOTIVELEG_H
#define _LOCOMOTIVELEG_H

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

#include "Joint.h"

using namespace std;

class LocomotiveLeg
{
private:
	float curTime;
	glm::vec3 cur_goal;
	glm::vec3 cur_goal_world;
	Joint * root;
	Joint * hip;
	Joint * leg;
	vector<glm::vec3> lerp_points;
	glm::vec3 lerp_point1;
	glm::vec3 lerp_point2;
	glm::vec3 lerp_point3;
	glm::vec3 lerp_point4;

public:
	LocomotiveLeg(Joint * root, vector<glm::vec3> lerp_points, Joint * hip);
	~LocomotiveLeg();

	void Update(float curTime, float dutyFactor);
	float Lerp(float y1, float y2, float u);

	float CalcBezier(int coord, float u);

	void CalcIK();
};

#endif
