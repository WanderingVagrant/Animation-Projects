#include "..\include\Skeleton.h"
#include <iostream>

Skeleton::Skeleton()
{
	this->root = NULL;
}

Skeleton::~Skeleton()
{
	delete this->root;
}

void Skeleton::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	root->Draw(viewProjMtx, shader);
}

void Skeleton::Update()
{
	root->Update(glm::mat4(1.0f));
}

bool Skeleton::Load(const char* file, Tokenizer &token) {

	//std::cout << "Loading skel\n";
	// Parse tree
	root = new Joint();
	root->Load(token);
	root->getJoints(joints);
	return true;
}
