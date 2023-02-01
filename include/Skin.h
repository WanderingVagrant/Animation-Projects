#pragma once
#include "core.h"
#include "Tokenizer.h"
#include "Skeleton.h"
#include <vector>
#include <tuple>

class Skin {
private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	std::vector<glm::mat4> bindinginv;
	std::vector<std::vector<std::tuple<int, float>>> weights;

	glm::mat4 model;
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;


	int numverts;
	int numtri;

public:
	Skeleton* myskel;
	Skin();
	~Skin();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update();

	bool Load(const char * file, Tokenizer &token);
};