#ifndef _SKIN_H
#define _SKIN_H

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "token.h"
#include "Window.h"

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//from proj2
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace std;

class Skin
{
private:
	Tokenizer * token;

	Skeleton * skeleton;

	glm::mat4 toWorld;

	// These variables are needed for the shader program
	GLuint skinVBO, skinVAO, skinEBO;
	GLuint uProjection, uModel, uModelColor, uView;

	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector <glm::vec3> skinweights;
	vector<unsigned int> indices;
	vector<glm::mat4> bindings;
	vector<glm::mat4> worldMatrices;

	glm::mat4 dirLightToWorld;

	glm::vec3 dirLightDirection1;
	glm::vec3 dirLightDirection2;

	glm::vec3 outDirLightDirection;

	glm::vec3 diffuseMaterial;
	glm::vec3 specularMaterial;
	glm::vec3 ambientMaterial;
	float shininessMaterial;

	const vector<glm::vec3> materialData =
	{
		{ 0.7f, 0.7f, 0.7f },
		{ 0.7f,	0.7f, 0.7f },
		{ 0.5f, 0.5f, 0.5f }
	};
	float shininess = 0.2f;


public:
	Skin(Skeleton * skeleton);
	~Skin();
	
	bool Load(const char * file);
	void Draw(GLuint shaderProgram);
	void Update();
	void GenerateVAO();
};

#endif
