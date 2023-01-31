#include "Skin.h"

Skin::Skin()
{
	myskel = NULL;
}

Skin::~Skin()
{
}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
}

void Skin::Update()
{
}

bool Skin::Load(const char* file, Tokenizer& token)
{
	return false;
}
