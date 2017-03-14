#ifndef _JOINT_H_
#define _JOINT_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "token.h"
#include "DOF.h"

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//from proj2
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace std;

class Joint
{
private:

	vector<Joint*> allJoints;

	Joint * root;
	char name;
	Joint * parent;
	vector<Joint *> children;
	vector<DOF *> DOFs;
	glm::vec3 offset;
	const int XDOF = 0;
	const int YDOF = 1;
	const int ZDOF = 2;

	float boxxmin, boxymin, boxzmin, boxxmax, boxymax, boxzmax;

	glm::mat4 toWorld;
	glm::mat4 toLocal;
	//GLuint shaderProgram;
	glm::vec3 color = { 0.0f,0.0f,0.0f };

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModel, uModelColor, uView;

	/*GLfloat vertices[8][3] = {
		// "Front" vertices
		{ -2.0, -2.0,  2.0 },{ 2.0, -2.0,  2.0 },{ 2.0,  2.0,  2.0 },{ -2.0,  2.0,  2.0 },
		// "Back" vertices
		{ -2.0, -2.0, -2.0 },{ 2.0, -2.0, -2.0 },{ 2.0,  2.0, -2.0 },{ -2.0,  2.0, -2.0 }
	};*/

	vector<glm::vec3> vertices;

	const GLuint solidIndices[6][6] = {
		// Front face
		{ 0, 1, 2, 2, 3, 0 },
		// Top face
		{ 1, 5, 6, 6, 2, 1 },
		// Back face
		{ 7, 6, 5, 5, 4, 7 },
		// Bottom face
		{ 4, 0, 3, 3, 7, 4 },
		// Left face
		{ 4, 5, 1, 1, 0, 4 },
		// Right face
		{ 3, 2, 6, 6, 7, 3 }
	};

	const GLuint wireIndices[3][8] = {
		// Front face
		{ 0, 1, 1, 2, 2, 3, 3, 0 },
		// Back Face
		{ 4, 5, 5, 6, 6, 7, 7, 4 },
		// Sides
		{ 0, 4, 1, 5, 2, 6, 3, 7 }
	};

public:
	Joint(Joint * root);
	~Joint();
	
	bool Load(Tokenizer &token);
	bool AddChild(Joint * child);
	bool SetParent(Joint * parent);
	bool SetName(char name);
	void GetTree();
	void Draw(GLuint shaderProgram);
	void CalcVertices();
	glm::mat4 getWorldMat();
	void CalcMat();
	void Update();
	void AddJoint(Joint * joint);
	glm::mat4 FindWorldMat(int index);
	int GetJointCount();
	void SetDOF(int DOFtype, float change);
	void FindDOF(int jointIndex, int DOFtype, float change);
};

#endif
