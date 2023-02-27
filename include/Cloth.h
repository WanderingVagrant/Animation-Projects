#pragma once
#include "core.h"
class Cloth {
public:
	void update();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
};