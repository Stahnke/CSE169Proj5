#include "joint.h"
#include "Window.h"

Joint::Joint(Joint * root) {
	DOFs.push_back(new DOF());	//x
	DOFs.push_back(new DOF());	//y
	DOFs.push_back(new DOF());	//z

	toWorld = glm::mat4(1.0f);
	toLocal = glm::mat4(1.0f);

	if (root == NULL)
	{
		this->root = this;
		AddJoint(this);
	}
	else
	{
		this->root = root;
	}
}

Joint::~Joint() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

bool Joint::Load(Tokenizer &token) {
	token.FindToken("{");
	while (1) {
		char temp[256];
		token.GetToken(temp);

		//check for data tokens
		if (strcmp(temp, "offset") == 0) {
			offset.x = token.GetFloat();
			offset.y = token.GetFloat();
			offset.z = token.GetFloat();
		}

		else if (strcmp(temp, "boxmin") == 0) {
			boxxmin = token.GetFloat();
			boxymin = token.GetFloat();
			boxzmin = token.GetFloat();
		}

		else if (strcmp(temp, "boxmax") == 0) {
			boxxmax = token.GetFloat();
			boxymax = token.GetFloat();
			boxzmax = token.GetFloat();
		}

		else if (strcmp(temp, "rotxlimit") == 0) {
			float min = token.GetFloat();
			float max = token.GetFloat();
			DOFs[XDOF]->SetMinMax(min, max);
		}

		else if (strcmp(temp, "rotylimit") == 0) {
			float min = token.GetFloat();
			float max = token.GetFloat();
			DOFs[YDOF]->SetMinMax(min, max);
		}

		else if (strcmp(temp, "rotzlimit") == 0) {
			float min = token.GetFloat();
			float max = token.GetFloat();
			DOFs[ZDOF]->SetMinMax(min, max);
		}

		else if (strcmp(temp, "pose") == 0) {
			DOFs[XDOF]->SetValue(token.GetFloat());
			DOFs[YDOF]->SetValue(token.GetFloat());
			DOFs[ZDOF]->SetValue(token.GetFloat());
		}

		//make child joint
		else if (strcmp(temp, "balljoint") == 0) {
			Joint * joint = new Joint(root);
			root->AddJoint(joint);
			joint->SetName('c');
			joint->SetParent(this);
			joint->Load(token);
			AddChild(joint);
		}
		
		//end
		else if (strcmp(temp, "}") == 0)
		{
			CalcVertices();
			return true;
		}

		else token.SkipLine(); // Unrecognized token
	}
}

bool Joint::AddChild(Joint * child) {
	children.push_back(child);
	return true;
}

bool Joint::SetParent(Joint * parent) {
	this->parent = parent;
	return true;
}

bool Joint::SetName(char name) {
	this->name = name;
	return true;
}

void Joint::GetTree() {
	cout << "Name: " << name << endl;
	cout << "Offset: x = " << offset.x << " y = " << offset.y << " z = " << offset.z << endl;
	cout << "boxmin: x = " << boxxmin << " y = " << boxymin << " z = " << boxzmin << endl;
	cout << "boxmax: x = " << boxxmax << " y = " << boxymax << " z = " << boxzmax << endl;
	cout << "rotxlimit: " << DOFs[0]->GetMin() << " " << DOFs[0]->GetMax() << endl;
	cout << "rotylimit: " << DOFs[1]->GetMin() << " " << DOFs[1]->GetMax() << endl;
	cout << "rotzlimit: " << DOFs[2]->GetMin() << " " << DOFs[2]->GetMax() << endl;
	cout << "pose: " << DOFs[0]->GetValue() << " " << DOFs[1]->GetValue() << " " << DOFs[2]->GetValue() << endl;
	cout << endl;

	for each(Joint * child in children)
	{
		child->GetTree();
	}
}

void Joint::Draw(GLuint shaderProgram) {

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	GLuint uColorLoc = glGetUniformLocation(shaderProgram, "myColor");
	glUniform3f(uColorLoc, color.x, color.y, color.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	for each(Joint * child in children)
	{
		child->Draw(shaderProgram);
	}
}

void Joint::Update() {

	CalcMat();
	for each(Joint * child in children)
	{
		child->Update();
	}
}

void Joint::CalcVertices() {

	vertices.clear();

	vertices.push_back(glm::vec3{ boxxmin, boxymin,boxzmin });
	vertices.push_back(glm::vec3{ boxxmax, boxymin,boxzmin });
	vertices.push_back(glm::vec3{ boxxmax, boxymax,boxzmin });
	vertices.push_back(glm::vec3{ boxxmin, boxymax,boxzmin });
	vertices.push_back(glm::vec3{ boxxmin, boxymin,boxzmax });
	vertices.push_back(glm::vec3{ boxxmax, boxymin,boxzmax });
	vertices.push_back(glm::vec3{ boxxmax, boxymax,boxzmax });
	vertices.push_back(glm::vec3{ boxxmin, boxymax,boxzmax });

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireIndices), &wireIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::mat4 Joint::getWorldMat() {
	return toWorld;
}

void Joint::CalcMat() {

	toLocal = glm::mat4(1.0f);

	float xDeg = DOFs[XDOF]->GetValue() * 60.0f;
	float yDeg = DOFs[YDOF]->GetValue() * 60.0f;
	float zDeg = DOFs[ZDOF]->GetValue() * 60.0f;

	if (parent != NULL)
	{
		toLocal = glm::rotate(glm::mat4(1.0f), xDeg / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * toLocal;
		toLocal = glm::rotate(glm::mat4(1.0f), yDeg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * toLocal;
		toLocal = glm::rotate(glm::mat4(1.0f), zDeg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * toLocal;
		toLocal = glm::translate(glm::mat4(1.0f), offset) * toLocal;

		toWorld = parent->getWorldMat() * toLocal;
	}

	else
	{
		toLocal = glm::rotate(glm::mat4(1.0f), xDeg / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * toLocal;
		toLocal = glm::rotate(glm::mat4(1.0f), yDeg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * toLocal;
		toLocal = glm::rotate(glm::mat4(1.0f), zDeg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * toLocal;
		toLocal = glm::translate(glm::mat4(1.0f), offset) * toLocal;
		
		toWorld = toLocal;
	}
}

void Joint::AddJoint(Joint * joint) {
	allJoints.push_back(joint);
}

glm::mat4 Joint::FindWorldMat(int index) {
	return allJoints[index]->getWorldMat();
}

int Joint::GetJointCount() {
	return allJoints.size();
}

void Joint::SetDOF(int DOFtype, float change) {
	DOFs[DOFtype]->SetValue(change);
}

void Joint::FindDOF(int jointIndex, int DOFtype, float change) {
	allJoints[jointIndex]->SetDOF(DOFtype, change);

}

Joint * Joint::FindJoint(int index) {
	return children[index];
}

glm::vec3 Joint::GetOffset() {
	return offset;
}

void Joint::AddOffset(glm::vec3 transVec) {
	offset += transVec;
}

float Joint::GetDOF(int index) {
	return DOFs[index]->GetValue();
}