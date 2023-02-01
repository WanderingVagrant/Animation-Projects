#include "Skin.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

Skin::Skin()
{
	myskel = NULL;
	numverts = 0;
	numtri = 0;
}

Skin::~Skin()
{
}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// actiavte the shader program
	glUseProgram(shader);
	glm:: vec3 color = glm::vec3(0.5f, 0.95f, 0.2f);
	// get the locations and send the uniforms to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);


	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Skin::Update()
{
	
	if (myskel == NULL) {
		//std::cout << "NoskelNoupdat\n\n\n";
		return;
	}
	int i;

	std::vector<glm::mat4> skinning = std::vector<glm::mat4>();
	std::vector<glm::vec3> newpos = std::vector<glm::vec3>();
	std::vector<glm::vec3> newnorm = std::vector<glm::vec3>();

	for (i = 0; i < bindinginv.size(); ++i) {
		glm::mat4 skinner = (myskel->joints[i]->World) * bindinginv[i];
		

		//For scale or shear compute inverse transpose
		//skinner = glm::transpose(glm::inverse(skinner));
		
		
		skinning.push_back(skinner);
	}

	for (i = 0; i < positions.size(); ++i) {
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 norm = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec4 longpos = glm::vec4(positions[i], 1);
		glm::vec4 longnorm = glm::vec4(normals[i], 0);

		//Sum over weights
		for (std::tuple<int, float> w : weights[i]) {
			int jindex = std::get<0>(w);
			float weight = std::get<1>(w);
			pos = pos + (weight * glm::vec3(skinning[jindex] * longpos));
			norm = norm + (weight * glm::vec3(skinning[jindex] * longnorm));
		}
		newpos.push_back(pos);
		newnorm.push_back(glm::normalize(norm));
	}

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * newpos.size(), newpos.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * newnorm.size(), newnorm.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}

bool Skin::Load(const char* file, Tokenizer& token)
{
	model = glm::mat4(1.0f);
	int i = 0;
	int j = 0;
	//Parse positions
	numverts = token.GetInt();
	token.FindToken("{");
	for (i = 0; i < numverts; ++i) {
		float x = token.GetFloat();
		float y = token.GetFloat();
		float z = token.GetFloat();
		positions.push_back(glm::vec3(x, y, z));
	}
	token.FindToken("}");

	//Parse Normals
	token.FindToken("{");
	for (i = 0; i < numverts; ++i) {
		float x = token.GetFloat();
		float y = token.GetFloat();
		float z = token.GetFloat();
		normals.push_back(glm::vec3(x, y, z));
	}
	token.FindToken("}");
	
	//Skinweights
	int numw;
	token.FindToken("{");
	for (i = 0; i < numverts; ++i) {
		numw = token.GetInt();
		std::vector<std::tuple<int, float>> vweight = std::vector<std::tuple<int, float>>();
		for (j = 0; j < numw; ++j) {
			int x = token.GetInt();
			float y = token.GetFloat();
			vweight.push_back(std::tuple<int, float>(x, y));
		}
		weights.push_back(vweight);
	}

	//Triangle indicies
	token.FindToken("triangles");
	numtri = token.GetInt();
	token.FindToken("{");
	for (i = 0; i < numtri*3; ++i) {
		indices.push_back(token.GetInt());
	}
	token.FindToken("}");

	//Binding matricies
	token.FindToken("bindings");
	int numjoints = token.GetInt();
	token.FindToken("{");
	for (i = 0; i < numjoints; ++i) {
		token.FindToken("{");
		glm::mat4 bind = glm::mat4(1.0f);
		int r;
		int c;
		for (c = 0; c < 4; ++c) {
			for (r = 0; r < 3; ++r) {
				bind[c][r] = token.GetFloat();
			}
		}
		bind[0][3] = 0.0f;
		bind[1][3] = 0.0f;
		bind[2][3] = 0.0f;
		bind[3][3] = 1.0f;
		bindinginv.push_back(glm::inverse(bind));
	}

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}
