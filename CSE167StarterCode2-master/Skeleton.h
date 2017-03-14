#ifndef _SKELETON_H
#define _SKELETON_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "Joint.h"
#include "token.h"

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//from proj2
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace std;

class Skeleton
{
private:
	Joint * root;
	Tokenizer * token;
	vector<Joint*> joints;
	glm::mat4 mat = glm::mat4(1.0f);

public:
	Skeleton();
	~Skeleton();
	
	bool Load(const char * file);
	void Draw(GLuint shaderProgram);
	void Update();
	glm::mat4 FindWorldMat(int index);
	int GetJointCount();
	void SetDOF(int index, int DOFtype, float change);
	Joint * GetRoot();
};

#endif
