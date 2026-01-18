#include "Car.h"

Car::Car(glm::vec3 startPos, std::vector<GLuint> textures)
{
    this->position = startPos;
    this->rotationAngle = 0.0f;
    this->wheelAngle = 0.0f;

    // Textures: [0]=Body, [1]=Black/Tire, [2]=Glass, [3]=Grille/Metal
    GLuint bodyTex = textures[1];
    GLuint blackTex = textures[0];
    GLuint glassTex = textures[2];
    GLuint metalTex = textures[3];
    GLuint coverWheel = textures[4];

    // --- 1. CABIN SHELL (Hollow) ---
    // Floor
    floor = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    floor->setTexture(bodyTex);

    // Ceiling
    ceiling = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    ceiling->setTexture(bodyTex);

    // Side Walls (Thick enough to hold windows later)
    wallLeft = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    wallLeft->setTexture(bodyTex);

    wallRight = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    wallRight->setTexture(bodyTex);

    // Back Door
    wallBack = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    wallBack->setTexture(bodyTex);

    // Windshield (Glass)
    windshield = new Cube(glm::vec3(0), 1.0f, glm::vec4(0.2, 0.3, 0.5, 0.6));
    // windshield->transformation(glm :: rotate(windshield->getMatrixModel(), -30, glm::vec3(1, 0, 0)));
    windshield->setTexture(glassTex);

    // --- 2. FRONT END (Engine Bay) ---
    hood = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    hood->setTexture(bodyTex);

    grille = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    grille->setTexture(metalTex); // Use a distinct texture if available

    // --- 3. DETAILS ---
    bumperFront = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    bumperFront->setTexture(blackTex);

    bumperRear = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    bumperRear->setTexture(blackTex);

    runningBoardLeft = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    runningBoardLeft->setTexture(blackTex); // Metal step

    runningBoardRight = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    runningBoardRight->setTexture(blackTex);

    // Turn Signals (The small boxes on hood)
    turnSignalLeft = new Cube(glm::vec3(0), 1.0f, glm::vec4(1, 0.5, 0, 1)); // Orange
    turnSignalRight = new Cube(glm::vec3(0), 1.0f, glm::vec4(1, 0.5, 0, 1));

    // Fenders (4 Arches)
    for (int i = 0; i < 4; i++)
    {
        fenders.push_back(new Cube(glm::vec3(0), 1.0f, glm::vec4(1)));
        fenders.back()->setTexture(bodyTex); // Body colored fenders
    }

    // --- 4. WHEELS ---
    for (int i = 0; i < 4; i++)
    {
        wheels.push_back(new Cylinder(glm::vec3(0), 1.0f, 1.0f, glm::vec4(1), 24));
        wheels.back()->setTexture(blackTex);
        wheels.at(i)->faces[0].setTexture(coverWheel);
        wheels.at(i)->faces[1].setTexture(coverWheel);
    }

    spareWheel = new Cylinder(glm::vec3(0), 1.0f, 1.0f, glm::vec4(1), 24);
    spareWheel->setTexture(blackTex);

    spareWheelCover = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    spareWheelCover->setTexture(bodyTex);
}

Car::~Car()
{
    delete floor;
    delete ceiling;
    delete wallLeft;
    delete wallRight;
    delete wallBack;
    delete windshield;
    delete hood;
    delete grille;
    delete bumperFront;
    delete bumperRear;
    delete runningBoardLeft;
    delete runningBoardRight;
    delete turnSignalLeft;
    delete turnSignalRight;
    delete spareWheel;
    delete spareWheelCover;
    for (auto f : fenders)
        delete f;
    for (auto w : wheels)
        delete w;
}

void Car::move(glm::vec3 dir)
{
    position += dir;
    wheelAngle -= 0.1f; // Rotate wheels
}

void Car::rotate(float angle)
{
    rotationAngle += angle;
}

void Car::draw(Shader &shader)
{
    // Global Transformation
    glm::mat4 carModel = glm::mat4(1.0f);
    carModel = glm::translate(carModel, position);
    carModel = glm::rotate(carModel, glm::radians(rotationAngle), glm::vec3(0, 1, 0));


    // ===========================
    //       FRONT END
    // ===========================
    // 7. Hood (Engine Block) - Solid
    glm::mat4 hoodMat = glm::translate(carModel, glm::vec3(0.0f, 0.95f, -1.8f));
    hoodMat = glm::scale(hoodMat, glm::vec3(1.7f, 0.8f, 1.5f)); // Wide and flat
    hood->transformation(hoodMat);
    hood->draw(shader);

    // 8. Grille
    glm::mat4 grilleMat = glm::translate(carModel, glm::vec3(0.0f, 0.95f, -2.56f)); // Very front
    grilleMat = glm::scale(grilleMat, glm::vec3(1.5f, 0.6f, 0.05f));
    grille->transformation(grilleMat);
    grille->draw(shader);

    // 9. Turn Signals (On top of hood corners)
    glm::mat4 sigL = glm::translate(carModel, glm::vec3(-0.7f, 1.4f, -2.2f));
    sigL = glm::scale(sigL, glm::vec3(0.15f, 0.1f, 0.2f));
    turnSignalLeft->transformation(sigL);
    turnSignalLeft->draw(shader);

    glm::mat4 sigR = glm::translate(carModel, glm::vec3(0.7f, 1.4f, -2.2f));
    sigR = glm::scale(sigR, glm::vec3(0.15f, 0.1f, 0.2f));
    turnSignalRight->transformation(sigR);
    turnSignalRight->draw(shader);


    // 6. Windshield (Glass) - Nearly Vertical
    glm::mat4 wsMat = glm::translate(carModel, glm::vec3(0.0f, 1.5f, -1.0f)); // Front of cabin
    // wsMat = glm::rotate(wsMat, glm::radians(-10.0f), glm::vec3(1, 0, 0));     // 80 degree angle (G-class style)
    wsMat = glm::translate(wsMat, glm::vec3(0.0f, 0.2f, 0.0f));
    wsMat = glm::scale(wsMat, glm::vec3(1.7f, 0.7f, 0.05f));
    windshield->transformation(wsMat);
    windshield->draw(shader);


    // ===========================
    //       HOLLOW CABIN
    // ===========================
    // 1. Floor (Base of cabin)
    glm::mat4 floorMat = glm::translate(carModel, glm::vec3(0.0f, 0.6f, 0.5f)); // 0.6 up from ground
    floorMat = glm::scale(floorMat, glm::vec3(1.8f, 0.6f, 3.0f));               // Wide and Long
    floor->transformation(floorMat);
    floor->draw(shader);

    // 2. Ceiling (Roof)
    glm::mat4 roofMat = glm::translate(carModel, glm::vec3(0.0f, 2.1f, 0.5f));
    roofMat = glm::scale(roofMat, glm::vec3(1.8f, 0.1f, 3.0f));
    ceiling->transformation(roofMat);
    ceiling->draw(shader);

    // 3. Left Wall (Thin)
    glm::mat4 lwMat = glm::translate(carModel, glm::vec3(-0.85f, 1.35f, 0.5f));
    lwMat = glm::scale(lwMat, glm::vec3(0.1f, 1.5f, 3.0f));
    wallLeft->transformation(lwMat);
    wallLeft->draw(shader);

    // 4. Right Wall (Thin)
    glm::mat4 rwMat = glm::translate(carModel, glm::vec3(0.85f, 1.35f, 0.5f));
    rwMat = glm::scale(rwMat, glm::vec3(0.1f, 1.5f, 3.0f));
    wallRight->transformation(rwMat);
    wallRight->draw(shader);

    // 5. Back Wall (Rear Door)
    glm::mat4 backMat = glm::translate(carModel, glm::vec3(0.0f, 1.35f, 1.95f)); // Back of car
    backMat = glm::scale(backMat, glm::vec3(1.8f, 1.5f, 0.1f));
    wallBack->transformation(backMat);
    wallBack->draw(shader);


    // ===========================
    //       DETAILS
    // ===========================
    // 10. Bumpers
    glm::mat4 bFront = glm::translate(carModel, glm::vec3(0.0f, 0.45f, -2.6f));
    bFront = glm::scale(bFront, glm::vec3(1.8f, 0.3f, 0.15f));
    bumperFront->transformation(bFront);
    bumperFront->draw(shader);

    glm::mat4 bRear = glm::translate(carModel, glm::vec3(0.0f, 0.45f, 2.05f));
    bRear = glm::scale(bRear, glm::vec3(1.8f, 0.3f, 0.15f));
    bumperRear->transformation(bRear);
    bumperRear->draw(shader);

    // 11. Running Boards (Side Steps)
    glm::mat4 stepL = glm::translate(carModel, glm::vec3(-1.0f, 0.35f, 0.5f));
    stepL = glm::scale(stepL, glm::vec3(0.2f, 0.05f, 2.0f));
    runningBoardLeft->transformation(stepL);
    runningBoardLeft->draw(shader);

    glm::mat4 stepR = glm::translate(carModel, glm::vec3(1.0f, 0.35f, 0.5f));
    stepR = glm::scale(stepR, glm::vec3(0.2f, 0.05f, 2.0f));
    runningBoardRight->transformation(stepR);
    runningBoardRight->draw(shader);

    // 12. Wheels & Arches
    float wx[] = {-0.85f, 0.85f, -0.85f, 0.85f};
    float wz[] = {-1.6f, -1.6f, 1.2f, 1.2f}; // Wheelbase position

    for (int i = 0; i < 4; i++)
    {
        // Wheel
        glm::mat4 wMat = glm::translate(carModel, glm::vec3(wx[i], 0.45f, wz[i]));
        wMat = glm::rotate(wMat, glm::radians(90.0f), glm::vec3(0, 0, 1));
        wMat = glm::rotate(wMat, wheelAngle, glm::vec3(0, 1, 0));
        wMat = glm::scale(wMat, glm::vec3(0.45f, 0.3f, 0.45f));
        wheels[i]->transformation(wMat);
        wheels[i]->draw(shader);

        // Arch (Flare) - sits above wheel, slightly wider than body
        float flareOffset = (wx[i] < 0) ? -0.15f : 0.15f; // Stick out left or right
        glm::mat4 aMat = glm::translate(carModel, glm::vec3(wx[i] + flareOffset, 0.9f, wz[i]));
        aMat = glm::scale(aMat, glm::vec3(0.15f, 0.05f, 0.9f));
        fenders[i]->transformation(aMat);
        fenders[i]->draw(shader);
    }

    // 13. Spare Wheel (Rear)
    glm::mat4 swMat = glm::translate(carModel, glm::vec3(0.0f, 1.3f, 2.1f));
    swMat = glm::scale(swMat, glm::vec3(0.4f, 0.2f, 0.4f));
    spareWheel->transformation(swMat);
    spareWheel->draw(shader);

    // Spare Cover (Square or Cylinder, usually circle but cube looks like a rugged case)
    glm::mat4 scMat = glm::translate(carModel, glm::vec3(0.0f, 1.3f, 2.22f));
    scMat = glm::scale(scMat, glm::vec3(0.7f, 0.7f, 0.05f));
    spareWheelCover->transformation(scMat);
    spareWheelCover->draw(shader);


}
