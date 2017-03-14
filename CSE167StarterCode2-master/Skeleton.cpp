#include "Skeleton.h"

Skeleton::Skeleton() {
}

Skeleton::~Skeleton() {
}

bool Skeleton::Load(const char *file) {

	//open file
	token = new Tokenizer();
	token->Open(file);

	//parse tree
	root = new Joint(NULL);
	root->SetName('r');
	root->Load(*token);
	//root->GetTree();
	
	token->Close();
	return true;
}

void Skeleton::Draw(GLuint shaderProgram)
{
	root->Draw(shaderProgram);
}

void Skeleton::Update()
{
	root->Update();
}

glm::mat4 Skeleton::FindWorldMat(int index) {
	return root->FindWorldMat(index);
}

int Skeleton::GetJointCount() {
	return root->GetJointCount();
}

void Skeleton::SetDOF(int jointIndex, int DOFtype, float change) {
	root->FindDOF(jointIndex, DOFtype, change);
}

Joint * Skeleton::GetRoot() {
	return root;
}

