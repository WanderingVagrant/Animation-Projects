#pragma once
#include "core.h"
#include "Tokenizer.h"
#include "Skeleton.h"

class Skin {
private:
public:
	Skeleton* myskel;
	Skin();
	~Skin();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update();

	bool Load(const char * file, Tokenizer &token);
};