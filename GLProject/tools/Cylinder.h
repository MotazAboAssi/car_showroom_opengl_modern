#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Polygon.h" // Essential
#include "learnopengl/shader.h"
class Cylinder
{
public:
    // A cylinder is a collection of faces (Top Cap, Bottom Cap, and Side Quads)
    std::vector<Polygon> faces;

    glm::vec3 center;
    glm::vec4 color;
    glm::mat4 model;

    float radius;
    float height;
    int segments; // How smooth the circle is (e.g., 36)

    // Constructor
    Cylinder(glm::vec3 center, float radius, float height, glm::vec4 color, int segments = 36);

    // Transforms the cylinder
    void transformation(glm::mat4 t);

    void setTexture(GLuint texID);


    // Draws all faces
    void draw(Shader &shader);
};
