#include "..\include\Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
}

void Skeleton::Update()
{
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
