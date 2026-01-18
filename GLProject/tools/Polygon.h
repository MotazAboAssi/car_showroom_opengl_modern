#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h> // Or your OpenGL loader
#include <cstddef>     // For offsetof
#include "learnopengl/shader.h"
// #include "VertexCust.h"

struct Vertex {
    glm::vec3 Position;  // 12 bytes
    glm::vec2 TexCoords; // 8 bytes
    // Total = 20 bytes (tightly packed usually)
};


class Polygon
{
public:
    // 2. Store a vector of 'VertexCust' structs instead of just vec3
    std::vector<Vertex> vertices;
    glm::vec3 head;
    glm::vec4 color;
    glm::mat4 model;

    GLuint VAO, VBO;
    GLuint textureID; // To store the texture for this polygon

    // Updated Constructor
    Polygon(std::vector<Vertex> v, glm::vec4 c);
    Polygon();

    void transformation(glm::mat4 t);
    void setTexture(GLuint texID); // Helper to assign texture
    void draw(Shader &shader);
    void deleteBuffers();
};
