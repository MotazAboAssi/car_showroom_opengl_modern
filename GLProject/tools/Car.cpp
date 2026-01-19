#include "Car.h"

// C++ Best Practice: Use an Initializer List for member objects
Car::Car(glm::vec3 startPos, std::vector<GLuint> texturesOut, std::vector<GLuint> texturesInside)
    : outDesign(texturesOut),      // Initialize Out object immediately
      insideDesign(texturesInside) // Initialize Inside object immediately
{
    this->position = startPos;
    this->rotationAngle = 0.0f;
    this->wheelAngle = 0.0f;
}

// Destructor can be empty. The destructor for 'outDesign' and 'insideDesign' 
// will be called automatically when the Car is destroyed.
Car::~Car() {
}

void Car::move(glm::vec3 dir) {
    position += dir;
    wheelAngle -= 0.1f; // Animate wheels
}

void Car::rotate(float angle) {
    rotationAngle += angle;
}

void Car::draw(Shader &shader) {
    // 1. Calculate the main Car model matrix ONCE
    glm::mat4 carModel = glm::mat4(1.0f);
    carModel = glm::translate(carModel, position);
    carModel = glm::rotate(carModel, glm::radians(rotationAngle), glm::vec3(0, 1, 0));

    // 2. Pass this matrix to the parts so they draw relative to the car
    outDesign.draw(shader, carModel, wheelAngle);
    insideDesign.draw(shader, carModel);
}

void Car::setDoorOpenAngle(float angle) {
    outDesign.doorOpenAngle = glm::clamp(angle, 0.0f, 1.0f);
}

float Car::getDoorOpenAngle() {
    return outDesign.doorOpenAngle;
}
