#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Polygon.h" // Essential: Defines 'Polygon' and 'Vertex'

class Cube {
public:
    // A cube is just a collection of 6 Polygon faces
    std::vector<Polygon> faces;
    
    glm::vec3 center;
    glm::vec4 color;
    glm::mat4 model;
    float edge;

    // Constructor
    Cube(glm::vec3 center, float edge, glm::vec4 color);
    
    // Transforms the cube
    void transformation(glm::mat4 t);

    void setTexture(GLuint texID);

    glm::mat4 getMatrixModel();

    
    // Draws all 6 faces
    void draw(Shader& shader);
};
