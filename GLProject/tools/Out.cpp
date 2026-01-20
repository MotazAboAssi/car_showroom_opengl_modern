#include "Out.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

Out::Out(std::vector<GLuint> textures)
{
    GLuint blackTex = textures[0];
    GLuint bodyTex = textures[1];
    GLuint glassTex = textures[2];
    GLuint metalTex = textures[3];
    GLuint coverWheel = textures[4];

    // --- Base Structure ---
    floor = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    floor->setTexture(bodyTex);
    ceiling = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    ceiling->setTexture(bodyTex);
    wallBack = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    wallBack->setTexture(bodyTex);
    windshield = new Cube(glm::vec3(0), 1.0f, glm::vec4(0.5f, 0.5f, 0.5f, 0.4f));
    windshield->setTexture(glassTex);

    // --- Front End ---
    hood = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    hood->setTexture(bodyTex);
    grille = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    grille->setTexture(metalTex);
    turnSignalLeft = new Cube(glm::vec3(0), 1.0f, glm::vec4(1, 0.5, 0, 1));
    turnSignalRight = new Cube(glm::vec3(0), 1.0f, glm::vec4(1, 0.5, 0, 1));

    // --- Details ---
    bumperFront = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    bumperFront->setTexture(blackTex);
    bumperRear = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    bumperRear->setTexture(blackTex);
    runningBoardLeft = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    runningBoardLeft->setTexture(blackTex);
    runningBoardRight = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    runningBoardRight->setTexture(blackTex);
    mirrorLeft = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    mirrorLeft->setTexture(blackTex);
    mirrorRight = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    mirrorRight->setTexture(blackTex);

    // --- 4 Doors & Pillars ---
    doorFL = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    doorFL->setTexture(bodyTex);
    doorFR = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    doorFR->setTexture(bodyTex);
    doorRL = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    doorRL->setTexture(bodyTex);
    doorRR = new Cube(glm::vec3(0), 1.0f, glm::vec4(1));
    doorRR->setTexture(bodyTex);
    bPillarL = new Cube(glm::vec3(0), 1.0f, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    bPillarR = new Cube(glm::vec3(0), 1.0f, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    // --- Wheels ---
    for (int i = 0; i < 4; i++)
    {
        wheels.push_back(new Cylinder(glm::vec3(0), 1.0f, 1.0f, glm::vec4(1), 24));
        wheels.back()->setTexture(blackTex);
        wheels[i]->faces[0].setTexture(coverWheel);
        wheels[i]->faces[1].setTexture(coverWheel);
        fenders.push_back(new Cube(glm::vec3(0), 1.0f, glm::vec4(1)));
        fenders.back()->setTexture(bodyTex);
    }
    spareWheel = new Cylinder(glm::vec3(0), 1.0f, 1.0f, glm::vec4(1), 24);
    spareWheel->setTexture(blackTex);
    spareWheel->faces[0].setTexture(coverWheel);
}

Out::~Out()
{
    delete floor;
    delete ceiling;
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
    delete mirrorLeft;
    delete mirrorRight;
    delete doorFL;
    delete doorFR;
    delete doorRL;
    delete doorRR;
    delete bPillarL;
    delete bPillarR;
    delete spareWheel;
    for (auto f : fenders)
        delete f;
    for (auto w : wheels)
        delete w;
}

void Out::draw(Shader &shader, glm::mat4 carModel, float wheelAngle)
{
    float openRad = glm::radians(45.0f * doorOpenAngle);

    // 1. Static Body Parts
    hood->transformation(glm::scale(glm::translate(carModel, glm::vec3(0, 0.95f, -1.8f)), glm::vec3(1.7f, 0.8f, 1.5f)));
    hood->draw(shader);
    windshield->transformation(glm::scale(glm::translate(carModel, glm::vec3(0, 1.7f, -1.4f)), glm::vec3(1.7f, 0.7f, 0.05f)));
    windshield->draw(shader);
    floor->transformation(glm::scale(glm::translate(carModel, glm::vec3(0, 0.6f, 0.5f)), glm::vec3(1.799f, 0.6f, 3.999f)));
    floor->draw(shader);
    ceiling->transformation(glm::scale(glm::translate(carModel, glm::vec3(0, 2.05f, 0.5f)), glm::vec3(1.6f, 0.1f, 4.0f)));
    ceiling->draw(shader);
    wallBack->transformation(glm::scale(glm::translate(carModel, glm::vec3(0, 1.35f, 2.45f)), glm::vec3(1.6f, 1.3f, 0.1f)));
    wallBack->draw(shader);
    grille->transformation(glm::scale(glm::translate(carModel, glm::vec3(0, 0.95f, -2.56f)), glm::vec3(1.5f, 0.6f, 0.05f)));
    grille->draw(shader);

    // 2. B-Pillars 
    bPillarL->transformation(glm::scale(glm::translate(carModel, glm::vec3(-0.85f, 1.35f, 0.5f)), glm::vec3(0.1f, 1.5f, 0.15f)));
    bPillarL->draw(shader);
    bPillarR->transformation(glm::scale(glm::translate(carModel, glm::vec3(0.85f, 1.35f, 0.5f)), glm::vec3(0.1f, 1.5f, 0.15f)));
    bPillarR->draw(shader);

    // 3. FRONT LEFT DOOR
    glm::mat4 pFL = glm::rotate(glm::translate(carModel, glm::vec3(-0.85f, 0.6f, -1.45f)), -openRad, glm::vec3(0, 1, 0));
    // Lower
    doorFL->transformation(glm::scale(glm::translate(pFL, glm::vec3(0, 0.4f, 0.95f)), glm::vec3(0.12f, 0.8f, 1.9f)));
    doorFL->draw(shader);
    // Upper (Window)
    doorFL->transformation(glm::scale(glm::translate(pFL, glm::vec3(0, 1.15f, 0.95f)), glm::vec3(0.05f, 0.7f, 1.85f)));
    doorFL->draw(shader);
    // Mirror
    mirrorLeft->transformation(glm::scale(glm::translate(pFL, glm::vec3(-0.15f, 0.85f, 0.3f)), glm::vec3(0.2f, 0.15f, 0.1f)));
    mirrorLeft->draw(shader);

    // 4. FRONT RIGHT DOOR
    glm::mat4 pFR = glm::rotate(glm::translate(carModel, glm::vec3(0.85f, 0.6f, -1.45f)), openRad, glm::vec3(0, 1, 0));
    doorFR->transformation(glm::scale(glm::translate(pFR, glm::vec3(0, 0.4f, 0.95f)), glm::vec3(0.12f, 0.8f, 1.9f)));
    doorFR->draw(shader);
    doorFR->transformation(glm::scale(glm::translate(pFR, glm::vec3(0, 1.15f, 0.95f)), glm::vec3(0.05f, 0.7f, 1.85f)));
    doorFR->draw(shader);
    mirrorRight->transformation(glm::scale(glm::translate(pFR, glm::vec3(0.15f, 0.85f, 0.3f)), glm::vec3(0.2f, 0.15f, 0.1f)));
    mirrorRight->draw(shader);

    // 5. REAR LEFT DOOR 
    glm::mat4 pRL = glm::rotate(glm::translate(carModel, glm::vec3(-0.85f, 0.6f, 0.55f)), -openRad * 0.9f, glm::vec3(0, 1, 0));
    doorRL->transformation(glm::scale(glm::translate(pRL, glm::vec3(0, 0.4f, 0.95f)), glm::vec3(0.12f, 0.8f, 1.9f)));
    doorRL->draw(shader);
    doorRL->transformation(glm::scale(glm::translate(pRL, glm::vec3(0, 1.15f, 0.95f)), glm::vec3(0.05f, 0.7f, 1.85f)));
    doorRL->draw(shader);

    // 6. REAR RIGHT DOOR 
    glm::mat4 pRR = glm::rotate(glm::translate(carModel, glm::vec3(0.85f, 0.6f, 0.55f)), openRad * 0.9f, glm::vec3(0, 1, 0));
    doorRR->transformation(glm::scale(glm::translate(pRR, glm::vec3(0, 0.4f, 0.95f)), glm::vec3(0.12f, 0.8f, 1.9f)));
    doorRR->draw(shader);
    doorRR->transformation(glm::scale(glm::translate(pRR, glm::vec3(0, 1.15f, 0.95f)), glm::vec3(0.05f, 0.7f, 1.85f)));
    doorRR->draw(shader);

    // 7. Wheels & Accessories
    float wx[] = {-0.85f, 0.85f, -0.85f, 0.85f}, wz[] = {-1.6f, -1.6f, 1.8f, 1.8f};
    for (int i = 0; i < 4; i++)
    {
        glm::mat4 wM = glm::scale(glm::rotate(glm::rotate(glm::translate(carModel, glm::vec3(wx[i], 0.45f, wz[i])), glm::radians(90.0f), glm::vec3(0, 0, 1)), wheelAngle, glm::vec3(0, 1, 0)), glm::vec3(0.45f, 0.3f, 0.45f));
        wheels[i]->transformation(wM);
        wheels[i]->draw(shader);
        fenders[i]->transformation(glm::scale(glm::translate(carModel, glm::vec3(wx[i] + (wx[i] < 0 ? -0.05f : 0.05f), 0.9f, wz[i])), glm::vec3(0.5f, 0.05f, 0.9f)));
        fenders[i]->draw(shader);
    }
    spareWheel->transformation(glm::scale(glm::rotate(glm::translate(carModel, glm::vec3(0, 1.3f, 2.6f)), (float)M_PI / 2, glm::vec3(1, 0, 0)), glm::vec3(0.4f, 0.2f, 0.4f)));
    spareWheel->draw(shader);
}
