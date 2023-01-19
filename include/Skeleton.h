#pragma once
#include "core.h"
#include "Tokenizer.h"
#include "Joint.h"

class Skeleton {
private:
	Joint* root;
public:
	Skeleton();
	~Skeleton();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update();

	bool Load(const char * file);
};