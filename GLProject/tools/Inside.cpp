#include "Inside.h"

Inside::Inside(std::vector<GLuint> textures) {
    // Assuming texturesInside[0] is seat fabric, [1] is dashboard/plastic
    GLuint seatTex = textures.size() > 0 ? textures[0] : 0;
    GLuint plasticTex = textures.size() > 1 ? textures[1] : seatTex;

    glm::vec4 seatColor(1.0f); // Use texture color

    // Create 4 seats
    seatFL = new Cube(glm::vec3(0), 1.0f, seatColor); seatFL->setTexture(seatTex);
    seatFR = new Cube(glm::vec3(0), 1.0f, seatColor); seatFR->setTexture(seatTex);
    seatBL = new Cube(glm::vec3(0), 1.0f, seatColor); seatBL->setTexture(seatTex);
    seatBR = new Cube(glm::vec3(0), 1.0f, seatColor); seatBR->setTexture(seatTex);

    // Steering Wheel (Black plastic)
    steeringWheel = new Cylinder(glm::vec3(0), 0.2f, 0.05f, glm::vec4(0.1f), 12);
    steeringWheel->setTexture(plasticTex);
}

Inside::~Inside() {
    delete seatFL; delete seatFR;
    delete seatBL; delete seatBR;
    delete steeringWheel;
}

void Inside::draw(Shader& shader, glm::mat4 carModel) {
    // Seat scaling (standard size for all)
    glm::vec3 seatScale(0.5f, 0.6f, 0.6f);

    // Front Seats (Z = 0.0)
    glm::mat4 flMat = glm::translate(carModel, glm::vec3(-0.4f, 0.9f, 0.0f));
    flMat = glm::scale(flMat, seatScale);
    seatFL->transformation(flMat); seatFL->draw(shader);

    glm::mat4 frMat = glm::translate(carModel, glm::vec3(0.4f, 0.9f, 0.0f));
    frMat = glm::scale(frMat, seatScale);
    seatFR->transformation(frMat); seatFR->draw(shader);

    // Back Seats (Moved back to Z = 1.2 because cabin is longer)
    glm::mat4 blMat = glm::translate(carModel, glm::vec3(-0.4f, 0.9f, 1.2f));
    blMat = glm::scale(blMat, seatScale);
    seatBL->transformation(blMat); seatBL->draw(shader);

    glm::mat4 brMat = glm::translate(carModel, glm::vec3(0.4f, 0.9f, 1.2f));
    brMat = glm::scale(brMat, seatScale);
    seatBR->transformation(brMat); seatBR->draw(shader);

    // Steering Wheel (Angled in front of driver)
    glm::mat4 steerMat = glm::translate(carModel, glm::vec3(-0.4f, 1.1f, -0.4f));
    steerMat = glm::rotate(steerMat, glm::radians(30.0f), glm::vec3(1, 0, 0)); // Tilt up
    steerMat = glm::rotate(steerMat, glm::radians(90.0f), glm::vec3(1, 0, 0)); // Face driver
    steeringWheel->transformation(steerMat);
    steeringWheel->draw(shader);
}
