#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Out.h"
#include "Inside.h" // Include the new header

class Car
{
public:
    // State
    glm::vec3 position;
    float rotationAngle;
    float wheelAngle;
    float doorOpenAngle = 0.0f; // Ensure this is public

    // Constructor & Destructor
    Car(glm::vec3 startPos, std::vector<GLuint> texturesOut, std::vector<GLuint> texturesInside);
    ~Car();

    // Functions
    void move(glm::vec3 dir);
    void rotate(float angle);
    void draw(Shader &shader);
    void setDoorOpenAngle(float angle);
    float getDoorOpenAngle();

private:
    // Composition: Car HAS-A Out and HAS-A Inside
    Out outDesign;
    Inside insideDesign;
};
