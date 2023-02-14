#pragma once
#include "core.h"
#include "Tokenizer.h"
#include "Joint.h"
#include "Animation.h"

class Skeleton {
private:
	Joint* root;
public:
	std::vector<Joint*> joints;
	Animation* anim;

	Skeleton();
	~Skeleton();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update();
	bool Load(Tokenizer& token);
};