#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"
#include "Cylinder.h"

class Out
{
public:
    // State
    // glm::vec3 position;
    // float rotationAngle;
    // float wheelAngle;
    Cube *wallLeft;
    Cube *wallRight;
    Cube *doorFL, *doorFR, *doorRL, *doorRR;
    Cube *bPillarL, *bPillarR;

        float doorOpenAngle = 0.0f; 


    // --- HOLLOW CABIN (Walls) ---
    Cube *floor;
    Cube *ceiling;
    Cube *wallBack;   // Rear door
    Cube *windshield; // Front glass

    // --- EXTERIOR BODY ---
    Cube *hood;   // Engine block (Solid)
    Cube *grille; // Front Grille
    Cube *bumperFront;
    Cube *bumperRear;

    // --- G-CLASS DETAILS ---
    std::vector<Cube *> fenders; // 4 Wheel arches (The flared part)
    Cube *runningBoardLeft;      // Side step
    Cube *runningBoardRight;
    Cube *turnSignalLeft; // The iconic box lights on the hood
    Cube *turnSignalRight;
    // NEW: Side Mirrors
    Cube *mirrorLeft;
    Cube *mirrorRight;

    // --- WHEELS ---
    std::vector<Cylinder *> wheels;
    Cylinder *spareWheel;

    // Constructor & Destructor
    Out(std::vector<GLuint> textures);
    ~Out();

    void draw(Shader &shader, glm::mat4 carModel, float wheelAngle);
};
