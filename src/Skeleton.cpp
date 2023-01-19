#include "..\include\Skeleton.h"

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

bool Skeleton::Load(const char* file) {
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");
	// Parse tree
	root = new Joint();
	root->Load(token);
	// Finish
	token.Close();
	return true;
}
