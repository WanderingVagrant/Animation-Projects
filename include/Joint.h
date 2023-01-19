#pragma once
#include "core.h"
#include "Cube.h"
#include <vector>
#include "Tokenizer.h"

class Joint {
public:
	glm::mat4 Local;
	glm::mat4 World;
	Cube* cube;

	struct DOF {
		float value;
		float min;
		float max;
	};

	std::vector<DOF> dofs;

	Joint();
	~Joint();
	void Update();
	bool Load(Tokenizer& t);
	void AddChild();
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);

};