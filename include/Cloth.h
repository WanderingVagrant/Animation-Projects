#pragma once
#include "core.h"
#include <vector>
#include "Particle.h"
#include <random>
#include "Triangle.h"
#include "SpringDamper.h"

class Cloth {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Cloth Information
    std::vector<glm::vec3> positions;
    std::vector<Particle> particles;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<Triangle> triangles;
    std::vector<SpringDamper> sdamps;
    float mass;

    int numparts;
public:
	void update();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void shiftx(float shift);
    void shifty(float shift);
    void shiftz(float shift);
    Cloth(float width, float height, int  cols, int rows);
};