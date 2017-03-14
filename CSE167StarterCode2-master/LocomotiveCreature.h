#ifndef _LOCOMOTIVECREATURE_H
#define _LOCOMOTIVECREATURE_H

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

#include "Gait.h"
#include "LocomotiveLeg.h"

using namespace std;

class LocomotiveCreature
{
private:
	Gait * walk_gait;
	Gait * gallop_gait;
	Gait * cur_gait;
	Joint * root;

	vector<LocomotiveLeg*> legs;

public:
	LocomotiveCreature(Joint * root);
	~LocomotiveCreature();

	void Update(float stepSize);
};

#endif
