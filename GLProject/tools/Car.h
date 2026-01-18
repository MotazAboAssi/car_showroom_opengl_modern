#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"
#include "Cylinder.h"

class Car {
public:
    // The parts of the car
    Cube* chassis;
    std::vector<Cylinder*> wheels; // 4 wheels
    Cube* driverSeat;
    Cube* passSeat;
    Cylinder* steeringWheel;

    glm::vec3 position;
    float rotationAngle; // Car's rotation around Y axis
    float wheelAngle;    // Rotation of wheels (rolling)

    // Constructor: loads the parts
    Car(glm::vec3 startPos, std::vector<GLuint> textures);
    ~Car();

    // Move the car
    void move(glm::vec3 dir);
    void rotate(float angle);

    // Draw all parts
    void draw(Shader& shader);
};
