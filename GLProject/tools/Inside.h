#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"
#include "Cylinder.h"

class Inside
{
public:
    // 4 Seats
    Cube *seatFL; // Front Left (Driver)
    Cube *seatFR; // Front Right
    Cube *seatBL; // Back Left
    Cube *seatBR; // Back Right

    // Controls
    Cylinder *steeringWheel;
    // Add dashboard, pedals, etc. here later

    Cube *dashBase;
    Cube *dualScreen;
    Cube *grabHandleBase;
    Cylinder *grabHandleBar;
    std::vector<Cylinder *> vents;

    // NEW: Gearshift assembly
    Cylinder *gearShaft;
    Cylinder *gearHead;

    Cube* clutchPedal;
    Cube* brakePedal;
    Cube* accelPedal;

    Inside(std::vector<GLuint> textures);
    ~Inside();

    // Also needs the parent car's model matrix
    void draw(Shader &shader, glm::mat4 carModel);
};
