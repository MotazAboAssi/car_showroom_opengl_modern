#include "Car.h"

Car::Car(glm::vec3 startPos, std::vector<GLuint> textures) {
    this->position = startPos;
    this->rotationAngle = 0.0f;
    this->wheelAngle = 0.0f;

    // --- 1. CHASSIS (Main Body) ---
    // A flat long rectangle. 
    // Size: Width 2.0, Height 0.5, Length 4.0
    // We create it at 0,0,0 relative to the car
    chassis = new Cube(glm::vec3(0, 0.5, 0), 1.0f, glm::vec4(1, 1, 1, 1)); 
    // Note: Your Cube size is 'edge', which makes a cube. To make a rectangular chassis, 
    // we will use Scaling in the draw() function, or we need a proper RectangularPrism class.
    // For now, let's use the 'model' matrix to scale the Cube into a Rectangle.
    
    // Assign Body Texture (e.g., textures[2] is wall/metal)
    chassis->setTexture(textures[2]); 

    // --- 2. WHEELS (4 Cylinders) ---
    // Radius 0.4, Width (Height) 0.4
    for(int i=0; i<4; i++) {
        Cylinder* wheel = new Cylinder(glm::vec3(0,0,0), 0.4f, 0.4f, glm::vec4(0.2, 0.2, 0.2, 1), 16);
        wheel->setTexture(textures[1]); // Use a darker texture or specific wheel texture
        wheels.push_back(wheel);
    }

    // --- 3. INTERIOR (Seats) ---
    driverSeat = new Cube(glm::vec3(0,0,0), 0.6f, glm::vec4(0.8, 0.2, 0.2, 1)); // Red Seat
    driverSeat->setTexture(textures[1]); 

    passSeat = new Cube(glm::vec3(0,0,0), 0.6f, glm::vec4(0.8, 0.2, 0.2, 1));
    passSeat->setTexture(textures[1]);

    // --- 4. STEERING WHEEL ---
    steeringWheel = new Cylinder(glm::vec3(0,0,0), 0.2f, 0.05f, glm::vec4(0,0,0,1), 12);
}

Car::~Car() {
    delete chassis;
    delete driverSeat;
    delete passSeat;
    delete steeringWheel;
    for(auto w : wheels) delete w;
}

void Car::move(glm::vec3 dir) {
    position += dir;
    // Simple animation: roll wheels when moving
    wheelAngle += 0.1f; 
}

void Car::rotate(float angle) {
    rotationAngle += angle;
}

void Car::draw(Shader& shader) {
    // 1. Calculate the Car's Main Transform (Global Position & Rotation)
    glm::mat4 carModel = glm::mat4(1.0f);
    carModel = glm::translate(carModel, position);
    carModel = glm::rotate(carModel, glm::radians(rotationAngle), glm::vec3(0, 1, 0));

    // --- DRAW CHASSIS ---
    glm::mat4 chassisModel = glm::scale(carModel, glm::vec3(2.0f, 0.5f, 4.0f)); // Stretch cube to make car body
    chassis->transformation(chassisModel);
    chassis->draw(shader);

    // --- DRAW WHEELS ---
    // Wheel positions relative to car center: X (+- 1.1), Y (0.4), Z (+- 1.2)
    float wheelX[] = { -1.1f, 1.1f, -1.1f, 1.1f }; // Left/Right
    float wheelZ[] = { -1.2f, -1.2f, 1.2f, 1.2f }; // Front/Back

    for(int i=0; i<4; i++) {
        glm::mat4 wheelMat = carModel;
        wheelMat = glm::translate(wheelMat, glm::vec3(wheelX[i], 0.4f, wheelZ[i]));
        
        // Rotate cylinder to lie on its side (90 deg around Z)
        wheelMat = glm::rotate(wheelMat, glm::radians(90.0f), glm::vec3(0, 0, 1));
        
        // Spin the wheel (Animation)
        wheelMat = glm::rotate(wheelMat, wheelAngle, glm::vec3(0, 1, 0)); // It rotates around Y because we already flipped it

        wheels[i]->transformation(wheelMat);
        wheels[i]->draw(shader);
    }

    // --- DRAW SEATS ---
    // Driver: Left (-0.5), Back (0.2), Up inside car
    glm::mat4 driverMat = glm::translate(carModel, glm::vec3(-0.5f, 0.6f, 0.2f));
    driverSeat->transformation(driverMat);
    driverSeat->draw(shader);

    // Passenger: Right (0.5)
    glm::mat4 passMat = glm::translate(carModel, glm::vec3(0.5f, 0.6f, 0.2f));
    passSeat->transformation(passMat);
    passSeat->draw(shader);

    // --- DRAW STEERING WHEEL ---
    // In front of driver
    glm::mat4 steerMat = glm::translate(carModel, glm::vec3(-0.5f, 0.9f, -0.4f));
    steerMat = glm::rotate(steerMat, glm::radians(30.0f), glm::vec3(1, 0, 0)); // Tilt it
    steerMat = glm::rotate(steerMat, glm::radians(90.0f), glm::vec3(1, 0, 0)); // Face driver
    steeringWheel->transformation(steerMat);
    steeringWheel->draw(shader);
}
