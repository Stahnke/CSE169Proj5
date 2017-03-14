#include "Skin.h"

Skin::Skin(Skeleton * skeleton) {
	this->skeleton = skeleton;
}

Skin::~Skin() {

}

bool Skin::Load(const char *file) {

	cout << "loading skin" << endl;
	//open file
	token = new Tokenizer();
	token->Open(file);

	while (1) {
		char temp[256];
		token->GetToken(temp);

		//check for data tokens
		if (strcmp(temp, "positions") == 0) {
			//Get vertsize
			const int size = token->GetInt();
			//cout << "VERTSIZE = " << size << endl;
			token->SkipLine();

			//Get all vertices
			float x, y, z;
			for (int i = 0; i < size; i++)
			{
				x = token->GetFloat();
				y = token->GetFloat();
				z = token->GetFloat();
				vertices.push_back({ x,y,z });
				//cout << i+1 << ": " << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << endl;
				token->SkipLine();
			}
		}

		//normals
		else if (strcmp(temp, "normals") == 0) {
			//Get vertsize
			const int size = token->GetInt();
			//cout << "NORMSIZE = " << size << endl;
			token->SkipLine();

			//Get all vertices
			float x, y, z;
			for (int i = 0; i < size; i++)
			{
				x = token->GetFloat();
				y = token->GetFloat();
				z = token->GetFloat();
				normals.push_back({ x,y,z });
				//cout << i + 1 << ": " << normals[i][0] << " " << normals[i][1] << " " << normals[i][2] << endl;
				token->SkipLine();
			}
		}

		//skinweights
		else if (strcmp(temp, "skinweights") == 0) {
			//Get size
			const int size = token->GetInt();
			//cout << "SKINWEIGHTS SIZE = " << size << endl;
			token->SkipLine();

			//Get all vertices
			int joint;
			float weight;
			int num;

			for (int i = 0; i < size; i++)
			{
				num = token->GetInt();
				//cout << i + 1 << ": ";
				for (int j = 0; j < num; j++)
				{
					joint = token->GetInt();
					weight = token->GetFloat();
					skinweights.push_back({ num, joint, weight });	
				}

				for (int k = 0; k < 4 - num; k++)
				{
					skinweights.push_back({num, 0.0f, 0.0f });
				}	
				token->SkipLine();
			}
		}

		//normals
		else if (strcmp(temp, "triangles") == 0) {
			//Get vertsize
			const int size = token->GetInt();
			//cout << "indices SIZE = " << size << endl;
			token->SkipLine();

			//Get all vertices
			int x, y, z;
			for (int i = 0; i < size; i++)
			{
				x = token->GetInt();
				y = token->GetInt();
				z = token->GetInt();
				indices.push_back(x);
				indices.push_back(y);
				indices.push_back(z);
				//cout << i + 1 << ": " << indices[i][0] << " " << indices[i][1] << " " << indices[i][2] << endl;
				token->SkipLine();
			}
		}

		//get bindings
		else if (strcmp(temp, "bindings") == 0) {
			//Get vertsize
			const int size = token->GetInt();
			//cout << "BINDINGS SIZE = " << size << endl;
			token->SkipLine();
			token->GetToken(temp);


			//Get all vertices
			while (strcmp(temp, "matrix") == 0)
			{
				token->SkipLine();
				float ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
				ax = token->GetFloat();
				ay = token->GetFloat();
				az = token->GetFloat();
				token->SkipLine();
				bx = token->GetFloat();
				by = token->GetFloat();
				bz = token->GetFloat();
				token->SkipLine();
				cx = token->GetFloat();
				cy = token->GetFloat();
				cz = token->GetFloat();
				token->SkipLine();
				dx = token->GetFloat();
				dy = token->GetFloat();
				dz = token->GetFloat();
				token->SkipLine();

				bindings.push_back(glm::inverse(glm::mat4({ ax,ay,az, 0 }, { bx, by, bz, 0 }, { cx, cy, cz, 0 }, { dx, dy, dz, 1 })));

				//cout << ax << " " << ay << " " << az << " " << endl;
				//cout << bx << " " << by << " " << bz << " " << endl;
				//cout << cx << " " << cy << " " << cz << " " << endl;
				//cout << dx << " " << dy << " " << dz << " " << endl;
				token->SkipLine();
				token->GetToken(temp);
			}

			GenerateVAO();
			return true;
		}

		else token->SkipLine(); // Unrecognized token
	}

	return true;
}

void Skin::GenerateVAO() {

	for(int i = 0; i < skinweights.size(); i++)
	{
		if (i % 4 == 0) cout << endl;
		glm::vec3 skinweight = skinweights[i];
		cout << skinweight.x << " " << skinweight.y << " " << skinweight.z << " ";
	}

	toWorld = glm::mat4(1.0f);

	dirLightToWorld = glm::mat4(1.0f);

	dirLightDirection1 = { -40.0f, -40.0f, 0.0f };
	dirLightDirection2 = { 40.0f, -40.0f, 0.0f };

	diffuseMaterial = materialData[0];
	specularMaterial = materialData[1];
	ambientMaterial = materialData[2];
	shininessMaterial = shininess;

	vector<glm::vec3> combinedVertices;

	for (int i = 0; i < vertices.size(); i++)
	{
		combinedVertices.push_back(vertices[i]);
		combinedVertices.push_back(normals[i]);
		combinedVertices.push_back(skinweights[i*4]);
		combinedVertices.push_back(skinweights[i*4+1]);
		combinedVertices.push_back(skinweights[i*4+2]);
		combinedVertices.push_back(skinweights[i*4+3]);
		//cout << skinweights[i*4].x << " " << skinweights[i * 4].y << " " << skinweights[i * 4].z << " ";
		//cout << skinweights[i*4+1].x << " " << skinweights[i * 4 + 1].y << " " << skinweights[i * 4 + 1].z << " ";
		//cout << skinweights[i*4+2].x << " " << skinweights[i * 4 + 2].y << " " << skinweights[i * 4 + 2].z << " ";
		//cout << skinweights[i*4+3].x << " " << skinweights[i * 4 + 3].y << " " << skinweights[i * 4 + 3].z << " ";
		//cout << endl;
	}

	glGenVertexArrays(1, &skinVAO);
	glGenBuffers(1, &skinVBO);
	glGenBuffers(1, &skinEBO);

	glBindVertexArray(skinVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skinVBO);

	glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(glm::vec3), &combinedVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(GLfloat), (GLvoid*)(12 * sizeof(GLfloat)));
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(GLfloat), (GLvoid*)(15 * sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skinEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Skin::Draw(GLuint shaderProgram)
{
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	//bindings
	GLint bindingsLoc = glGetUniformLocation(shaderProgram, "bindings");
	glUniformMatrix4fv(bindingsLoc, bindings.size(), GL_FALSE, &bindings[0][0][0]);

	//world Matrices
	GLint worldMatricesLoc = glGetUniformLocation(shaderProgram, "worldMatrices");
	glUniformMatrix4fv(worldMatricesLoc, worldMatrices.size(), GL_FALSE, &worldMatrices[0][0][0]);

	//material
	GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
	GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform3f(matDiffuseLoc, diffuseMaterial.x, diffuseMaterial.y, diffuseMaterial.z);
	glUniform3f(matSpecularLoc, specularMaterial.x, specularMaterial.y, specularMaterial.z);
	glUniform3f(matAmbientLoc, ambientMaterial.x, ambientMaterial.y, ambientMaterial.z);
	glUniform1f(matShineLoc, shininessMaterial);

	outDirLightDirection = glm::vec3(Window::V * dirLightToWorld * glm::vec4(dirLightDirection1, 1.0f));

	//Directional Light direction 1
	GLint DirLightDirectionLoc = glGetUniformLocation(shaderProgram, "dirLight1.direction");
	//Directional Light diffuse
	GLint DirLightDiffuseLoc = glGetUniformLocation(shaderProgram, "dirLight1.diffuse");
	//Directional Light specular
	GLint DirLightSpecularLoc = glGetUniformLocation(shaderProgram, "dirLight1.specular");
	//Directional Light ambient
	GLint DirLightAmbientLoc = glGetUniformLocation(shaderProgram, "dirLight1.ambient");
	
	glUniform3f(DirLightDirectionLoc, outDirLightDirection.x, outDirLightDirection.y, outDirLightDirection.z); // Also set light's color (white)
	glUniform3f(DirLightDiffuseLoc, 0.0f, 0.0f, 1.0f); // Also set light's color (white)
	glUniform3f(DirLightSpecularLoc, 0.0f, 0.0f, 1.0f); // Also set light's color (white)
	glUniform3f(DirLightAmbientLoc, 0.0f, 0.0f, 1.0f); // Also set light's color (white)

	outDirLightDirection = glm::vec3(Window::V * dirLightToWorld * glm::vec4(dirLightDirection2, 1.0f));
	
	//Directional Light direction 2
	DirLightDirectionLoc = glGetUniformLocation(shaderProgram, "dirLight2.direction");
	//Directional Light diffuse
	DirLightDiffuseLoc = glGetUniformLocation(shaderProgram, "dirLight2.diffuse");
	//Directional Light specular
	DirLightSpecularLoc = glGetUniformLocation(shaderProgram, "dirLight2.specular");
	//Directional Light ambient
	DirLightAmbientLoc = glGetUniformLocation(shaderProgram, "dirLight2.ambient");

	glUniform3f(DirLightDirectionLoc, outDirLightDirection.x, outDirLightDirection.y, outDirLightDirection.z); // Also set light's color (white)
	glUniform3f(DirLightDiffuseLoc, 1.0f, 0.0f, 0.0f); // Also set light's color (white)
	glUniform3f(DirLightSpecularLoc, 1.0f, 0.0f, 0.0f); // Also set light's color (white)
	glUniform3f(DirLightAmbientLoc, 1.0f, 0.0f, 0.0f); // Also set light's color (white)

	//view position
	glm::vec3 cam_pos = Window::getCamPos();
	GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, cam_pos.x, cam_pos.y, cam_pos.z);

	glBindVertexArray(skinVAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void Skin::Update()
{
	worldMatrices.clear();

	for (int i = 0; i < bindings.size(); i++)
	{
		worldMatrices.push_back(skeleton->FindWorldMat(i));
	}
}

