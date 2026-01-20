#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"
#include "Cylinder.h"

class Out
{
public:
    Cube *wallLeft;
    Cube *wallRight;
    Cube *doorFL, *doorFR, *doorRL, *doorRR;
    Cube *bPillarL, *bPillarR;

        float doorOpenAngle = 0.0f; 


    // --- HOLLOW CABIN (Walls) ---
    Cube *floor;
    Cube *ceiling;
    Cube *wallBack;   
    Cube *windshield; 

    // --- EXTERIOR BODY ---
    Cube *hood;   
    Cube *grille; 
    Cube *bumperFront;
    Cube *bumperRear;

    // --- G-CLASS DETAILS ---
    std::vector<Cube *> fenders; 
    Cube *runningBoardLeft;      
    Cube *runningBoardRight;
    Cube *turnSignalLeft; 
    Cube *turnSignalRight;

    Cube *mirrorLeft;
    Cube *mirrorRight;

    // --- WHEELS ---
    std::vector<Cylinder *> wheels;
    Cylinder *spareWheel;

    Out(std::vector<GLuint> textures);
    ~Out();

    void draw(Shader &shader, glm::mat4 carModel, float wheelAngle);
};
