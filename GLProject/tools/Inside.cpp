#include "Inside.h"

Inside::Inside(std::vector<GLuint> textures)
{
    // Assuming texturesInside[0] is seat fabric, [1] is dashboard/plastic
    GLuint seatTex = textures.size() > 0 ? textures[0] : 0;
    GLuint plasticTex = textures.size() > 1 ? textures[1] : seatTex;

    glm::vec4 seatColor(1.0f); // Use texture color

    // Create 4 seats
    seatFL = new Cube(glm::vec3(0), 1.0f, seatColor);
    seatFL->setTexture(seatTex);
    seatFR = new Cube(glm::vec3(0), 1.0f, seatColor);
    seatFR->setTexture(seatTex);
    seatBL = new Cube(glm::vec3(0), 1.0f, seatColor);
    seatBL->setTexture(seatTex);
    seatBR = new Cube(glm::vec3(0), 1.0f, seatColor);
    seatBR->setTexture(seatTex);

    dashBase = new Cube(glm::vec3(0), 1.0f, seatColor);
    dashBase->setTexture(plasticTex);
    dualScreen = new Cube(glm::vec3(0), 1.0f, seatColor);
    dualScreen->setTexture(textures[5]);
    grabHandleBase = new Cube(glm::vec3(0), 1.0f, seatColor);
    grabHandleBase->setTexture(seatTex);
    grabHandleBar = new Cylinder(glm::vec3(0), 1.0f, 1.0f, glm::vec4(1), 24);
    grabHandleBar->setTexture(seatTex);

    for (int i = 0; i < 4; i++)
    {
        vents.push_back(new Cylinder(glm::vec3(0), 1.0f, 1.0f, glm::vec4(1), 24));
        vents.back()->setTexture(textures[1]); // Body colored fenders
    }

    // Steering Wheel (Black plastic)

    // Steering Wheel
    steeringWheel = new Cylinder(glm::vec3(0), 0.25f, 0.04f, glm::vec4(1.0f), 24);
    steeringWheel->setTexture(textures[7]);
    steeringWheel->faces.at(0).setTexture(textures[6]);
    steeringWheel->faces.at(1).setTexture(textures[6]);
    // Gearshift: Shaft (Thin and metallic/black)
    gearShaft = new Cylinder(glm::vec3(0), 0.02f, 0.3f, glm::vec4(0.2f), 12);
    gearShaft->setTexture(textures[1]);

    // Gearshift: Head (The cylinder "knob")
    gearHead = new Cylinder(glm::vec3(0), 0.05f, 0.08f, glm::vec4(0.05f), 16);
    gearHead->setTexture(textures[1]);

    // Pedal Color: Dark Grey/Black
    glm::vec4 pedalColor(0.15f, 0.15f, 0.15f, 1.0f);

    // Initialize Pedals
    clutchPedal = new Cube(glm::vec3(0), 1.0f, pedalColor);
    brakePedal = new Cube(glm::vec3(0), 1.0f, pedalColor);
    accelPedal = new Cube(glm::vec3(0), 1.0f, pedalColor);
}

Inside::~Inside()
{

    delete gearShaft;
    delete gearHead;

    delete seatFL;
    delete seatFR;
    delete seatBL;
    delete seatBR;

    delete clutchPedal;
    delete brakePedal;
    delete accelPedal;

    delete dashBase;
    delete dualScreen;
    delete grabHandleBase;
    delete grabHandleBar;
    std::vector<Cylinder *> vents;
    for (auto vent : vents)
        delete vent;

    delete steeringWheel;
}

void Inside::draw(Shader &shader, glm::mat4 carModel)
{
    // Seat scaling (standard size for all)
    glm::vec3 seatScale(0.5f, 0.6f, 0.6f);

    // Front Seats (Z = 0.0)
    glm::mat4 flMat = glm::translate(carModel, glm::vec3(-0.4f, 0.9f, 0.0f));
    flMat = glm::scale(flMat, seatScale);
    seatFL->transformation(flMat);
    seatFL->draw(shader);

    glm::mat4 frMat = glm::translate(carModel, glm::vec3(0.4f, 0.9f, 0.0f));
    frMat = glm::scale(frMat, seatScale);
    seatFR->transformation(frMat);
    seatFR->draw(shader);

    // Back Seats (Moved back to Z = 1.2 because cabin is longer)
    glm::mat4 blMat = glm::translate(carModel, glm::vec3(-0.4f, 0.9f, 1.2f));
    blMat = glm::scale(blMat, seatScale);
    seatBL->transformation(blMat);
    seatBL->draw(shader);

    glm::mat4 brMat = glm::translate(carModel, glm::vec3(0.4f, 0.9f, 1.2f));
    brMat = glm::scale(brMat, seatScale);
    seatBR->transformation(brMat);
    seatBR->draw(shader);

    // ===========================
    //    G-CLASS DASHBOARD (Z ≈ -0.8f)
    // ===========================

    // 2. The Iconic Dual Screens (Infotainment + Gauges)
    // Use a special texture that has the speedometer and GPS in one image!
    glm::mat4 screenMat = glm::translate(carModel, glm::vec3(-0.1f, 1.25f, -1.05f));
    screenMat = glm::scale(screenMat, glm::vec3(1.3f, 0.4f, 0.02f)); // Wide and thin
    dualScreen->transformation(screenMat);
    dualScreen->draw(shader);

    // 2. STEERING WHEEL (Pushed close to the dashboard area)
    // Positioned at X: -0.45 (Driver side), Y: 1.15 (Height), Z: -0.9 (Deep Front)
    glm::mat4 steerMat = glm::translate(carModel, glm::vec3(-0.45f, 1.3f, -1.0f));
    steerMat = glm::rotate(steerMat, -(float)M_PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    steerMat = glm::scale(steerMat, glm::vec3(0.5f)); // Face driver
    steeringWheel->transformation(steerMat);
    steeringWheel->draw(shader);

    // 3. GEARSHIFT ASSEMBLY
    // Base position on the center console
    glm::vec3 gearBasePos = glm::vec3(0.0f, 0.7f, -0.3f);

    // Draw Shaft
    glm::mat4 shaftMat = glm::translate(carModel, gearBasePos);
    // No rotation needed if it stands straight up
    gearShaft->transformation(shaftMat);
    gearShaft->draw(shader);

    // Draw Head (Placed at the top of the shaft height)
    glm::mat4 headMat = glm::translate(carModel, gearBasePos + glm::vec3(0.0f, 0.15f, 0.0f));
    headMat = glm::rotate(headMat, glm::radians(90.0f), glm::vec3(0, 0, 1)); // Lay cylinder horizontally?
    gearHead->transformation(headMat);
    gearHead->draw(shader);

    //

    // ===========================
    //       PEDAL ASSEMBLY
    // ===========================
    // Shared parameters for the footwell
    float pedalY = 1.0f;                      // Slightly above the floor (0.6f)
    float pedalZ = -0.75f;                      // Deep in the front
    glm::vec3 pedalScale(0.10f, 0.13f, 0.0f); // Thin pads

    // 1. CLUTCH (Far Left)
    glm::mat4 clutchMat = glm::translate(carModel, glm::vec3(-0.7f, pedalY, pedalZ));
    clutchMat = glm::rotate(clutchMat, glm::radians(-20.0f), glm::vec3(1, 0, 0)); // Slanted
    clutchMat = glm::scale(clutchMat, pedalScale);
    clutchPedal->transformation(clutchMat);
    clutchPedal->draw(shader);

    // 2. BRAKE (Middle-Left)
    glm::mat4 brakeMat = glm::translate(carModel, glm::vec3(-0.5f, pedalY, pedalZ));
    brakeMat = glm::rotate(brakeMat, glm::radians(-20.0f), glm::vec3(1, 0, 0));
    brakeMat = glm::scale(brakeMat, pedalScale);
    brakePedal->transformation(brakeMat);
    brakePedal->draw(shader);

    // 3. ACCELERATOR (Far Right - often taller and narrower)
    glm::mat4 accelMat = glm::translate(carModel, glm::vec3(-0.3f, pedalY, pedalZ));
    accelMat = glm::rotate(accelMat, glm::radians(-20.0f), glm::vec3(1, 0, 0));
    accelMat = glm::scale(accelMat, glm::vec3(0.08f, 0.22f, 0.02f)); // Taller shape
    accelPedal->transformation(accelMat);
    accelPedal->draw(shader);
}
