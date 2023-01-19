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
	glm::vec3 Offset;
	glm::vec3 boxmin;
	glm::vec3 boxmax;

	struct DOF {
		float value;
		float min;
		float max;
	};

	std::vector<DOF> dofs;

	std::vector<Joint *> childs;

	Joint();
	~Joint();
	void Update(glm::mat4 pWorld);
	bool Load(Tokenizer& t);
	void AddChild(Joint * child);
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);

};