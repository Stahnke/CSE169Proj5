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
	float accel;
	float speed;
	bool sprintOkay;
	bool sprinting;
	float turnRadian;
	bool movementOkay;

	float maxWalkSpeed;
	float maxGallopSpeed;

	vector<LocomotiveLeg*> legs;

public:
	LocomotiveCreature(Joint * root);
	~LocomotiveCreature();

	void Update();
	void SetAccel(float accel);
	void ComeToStop(float stoppingAccel);
	void SetSprint(bool sprintOkay);
	void TurnCreature(float turnRadian);
	void SetMovementOkay(bool movementOkay);
};

#endif
