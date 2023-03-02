#pragma once
#include "core.h"
#include <vector>
class Ground {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Ground Information
    std::vector<glm::vec4> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

public:
    Ground();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
};