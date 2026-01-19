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

    // --- HOLLOW CABIN (Walls) ---
    Cube *floor;
    Cube *ceiling;
    Cube *wallLeft;
    Cube *wallRight;
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

    // Functions
    void move(glm::vec3 dir);
    void rotate(float angle);
    void draw(Shader &shader, glm::mat4 carModel, float wheelAngle);
};
