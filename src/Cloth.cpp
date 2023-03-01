#include "..\include\Cloth.h"

void Cloth::update()
{

    float timestep = glfwGetTime();

    glfwSetTime(0);
    //Compute forces
    
    for (Triangle & tri : triangles) {
        tri.computeForce();
    }
    
    for (SpringDamper const &sdamp : sdamps) {
        sdamp.ComputeForce();
    }
    

    //Add gravity acceleration and integrate motion, then resolve collisions
    for (Particle &part : particles) {
        part.update(timestep);
    }

    
    //Implement dynamic smooth shading by averaging triangle normals
    for (Triangle tri : triangles) {
        tri.computeNorm();
        tri.p1.normal += tri.normal;
        tri.p2.normal += tri.normal;
        tri.p3.normal += tri.normal;
    }
    for (Particle& part : particles) {
        part.normal = glm::normalize(part.normal);
    }
    

    //Send updated data
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

}

void Cloth::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
    // actiavte the shader program
    glUseProgram(shader);
    glm::vec3 color = glm::vec3(0.5f, 0.95f, 0.2f);
    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);


    // Bind the VAO
    glBindVertexArray(VAO);

    //Turn on wireframe mode
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    //Turn off wireframe mode
   // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

Cloth::Cloth()
{
    // Model matrix.
    model = glm::mat4(1.0f);

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    float width = 5;
    float height = 5;
    int cols = 15;
    int rows = 15;
    numparts = cols * rows;
    mass = PhysWorld::clothdensity * width * height;
    float partmass = mass / numparts;


    //Build Positions and normals
    positions.reserve(numparts);
    normals.reserve(numparts);
    particles.reserve(numparts);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool top = false;
            if (i == rows-1) {
                top = true;
            }
            positions.push_back(glm::vec3(width*(((float)j)/cols), height*(((float) i)/rows), 0));
            normals.push_back(glm::vec3(0, 1, 0));
            particles.push_back(Particle(positions.back(), normals.back(),partmass, top));
        }
    }

    // Build Triangles and Indicies and Spring damper
    triangles.reserve((cols - 1) * (rows - 1) * 2);
    sdamps.reserve((cols - 1) * (rows - 1) * 4 + (rows - 1));
    indices.reserve(triangles.size() * 3);
    for (int i = 0; i < rows-1; ++i) {
        for (int j = 0; j < cols-1; ++j) {
            //triangles
            triangles.push_back(Triangle(particles.at(i*cols + j), particles.at(i * cols + (j+1)), particles.at((i+1) * cols + (j + 1)) ));
            triangles.push_back(Triangle(particles.at(i * cols + j), particles.at((i + 1) * cols + (j + 1)), particles.at((i + 1) * cols + (j))));

            //indicies
            indices.insert(indices.end(), { (unsigned int) i * cols + j,(unsigned int)i * cols + (j + 1),(unsigned int)(i + 1) * cols + (j + 1),
                (unsigned int)i * cols + j,(unsigned int)(i + 1) * cols + (j + 1),(unsigned int)(i + 1) * cols + (j) });

            //SpringDampers
            sdamps.push_back(SpringDamper(particles.at(i * cols + j), particles.at(i * cols + (j + 1))));
            sdamps.push_back(SpringDamper(particles.at(i * cols + j), particles.at((i + 1) * cols + (j + 1))));
            sdamps.push_back(SpringDamper(particles.at(i * cols + j), particles.at((i + 1) * cols + (j))));
            sdamps.push_back(SpringDamper(particles.at(i * cols + (j + 1)), particles.at((i + 1) * cols + (j))));
            if (j == cols - 2) {
                sdamps.push_back(SpringDamper(particles.at(i * cols + (j+1)), particles.at((i + 1) * cols + (j + 1))));
            }
        }
    }
    
    for (Particle& part : particles) {
        float zoffset = ((float)rand()) / RAND_MAX * 0.02f - 0.01;
        float yoffset = ((float)rand()) / RAND_MAX * 0.02f - 0.01;
        float xoffset = ((float) rand()) / RAND_MAX * 0.02f - 0.01;
        part.position.x += xoffset;
        part.position.y += yoffset;
        part.position.z += zoffset;
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

    //Start timer
    glfwSetTime(0);
}
