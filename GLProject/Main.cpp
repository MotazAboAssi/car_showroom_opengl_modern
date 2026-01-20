#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
// #include <learnopengl/model.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "tools/Polygon.h"
#include "tools/Cube.h"
#include "tools/Cylinder.h"
#include "tools/Car.h"

using namespace std;
using namespace glm;

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

Camera camera(vec3(0.0f, 0.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool inCar = false;

void processInput(GLFWwindow *window, Car &car, Camera &camera, bool &inCar, float deltaTime);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

vector<GLuint> loadTextures(vector<string> paths, GLuint wrapOption = GL_REPEAT, GLuint filterOption = GL_LINEAR)
{
	vector<GLuint> textures = {};

	// CRITICAL FIX 1: Tell OpenGL to read byte-by-byte (essential for JPGs with odd widths)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (string path : paths)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// CRITICAL FIX 2: Set parameters AFTER binding the specific texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterOption);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterOption);

		int width, height, nrChannels;
		// Load data
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			// CRITICAL FIX 3: Check channels to determine Format (RGB vs RGBA)
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB; // JPG usually falls here
			else if (nrChannels == 4)
				format = GL_RGBA; // PNG usually falls here

			// Use 'format' variable instead of hardcoded GL_RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture: " << path << std::endl;
		}

		stbi_image_free(data);
		textures.push_back(texture);
	}

	return textures;
}

int main()
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lecture 6", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Shader allShader("./shaders/vs/L6.vs", "./shaders/fs/L6.fs");
	// Shader lightSourceShader("./shaders/vs/LightSource2.vs", "./shaders/fs/LightSource2.fs");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(true);

	vector<string> texturePaths = {};
	texturePaths.push_back("./textures/window.png");
	texturePaths.push_back("./textures/white.png");
	texturePaths.push_back("./textures/wall.jpg");
	texturePaths.push_back("./textures/floor.jpg");
	texturePaths.push_back("./textures/building.jpg");
	vector<GLuint> textures = loadTextures(texturePaths);
	vector<string> texturePathMainCar = {};
	texturePathMainCar.push_back("./textures/car/tire_rubber.png");
	texturePathMainCar.push_back("./textures/car/body_paint.png");
	texturePathMainCar.push_back("./textures/car/window_glass.png");
	texturePathMainCar.push_back("./textures/car/grille_metal.png");
	texturePathMainCar.push_back("./textures/car/cover_wheel.png");
	texturePathMainCar.push_back("./textures/car/dashboard_without__steering_wheel .png");
	texturePathMainCar.push_back("./textures/car/steering wheel.png");
	texturePathMainCar.push_back("./textures/car/cover steering wheel.png");
	vector<GLuint> textureMainCar = loadTextures(texturePathMainCar);

	// Cube cube(vec3(0.0f, 0.0f, 0.0f), 1.0f, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Cylinder cylinder(vec3(0.0f, 0.0f, -10.0f), 1.0f, 1.0f, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Car myCar(glm::vec3(0.0f, 0.0f, 0.0f), textureMainCar, textureMainCar);
	Car myCar3(glm::vec3(0.0f, 0.0f, 10.0f), textureMainCar, textureMainCar);

	float lastCarAngle = myCar.rotationAngle;
	while (!glfwWindowShouldClose(window))
	{
		allShader.use();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float time = (float)glfwGetTime();

		processInput(window, myCar, camera, inCar, deltaTime);

		// --- CAMERA SYNC CODE START ---

		if (inCar)
		{
			// --- 1. POSITION LOCK (Stick to the seat) ---
			// Same as before: Calculate where the driver's head is
			glm::vec3 driverHeadOffset = glm::vec3(-0.5f, 1.8f, 0.0f);

			// Create rotation matrix for position offset
			glm::mat4 carRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(myCar.rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 rotatedOffset = glm::vec3(carRotationMatrix * glm::vec4(driverHeadOffset, 1.0f));

			// Lock Camera Position to Car
			camera.Position = myCar.position + rotatedOffset;

			// --- 2. ROTATION SYNC (The Magic Part) ---

			// Calculate how much the car turned since the last frame
			float deltaAngle = myCar.rotationAngle - lastCarAngle;

			// Apply that turn to the camera.
			// This allows the car to turn the camera, BUT the mouse can ALSO turn the camera.
			// (We subtract deltaAngle because usually Camera Yaw and Object Rotation are inverted in logic)
			camera.Yaw -= deltaAngle;

			// NOTE: Do NOT set camera.Pitch. Let the mouse control that freely.

			// Manually update vectors so the changes take effect immediately
			camera.updateCameraVectors();
		}

		else
		{
			// --- WALKING MODE: Free Camera ---
			// No special code needed here, WASD in processInput handles it.
			// lastCarAngle = myCar.rotationAngle;
		}
		// Update the history for the next frame
		lastCarAngle = myCar.rotationAngle;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 projection = mat4(1.0f);
		projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		allShader.setMat4("projection", projection);

		mat4 view = mat4(1.0f);
		view = camera.GetViewMatrix();
		allShader.setMat4("view", view);

		cylinder.draw(allShader);
		
		
		myCar.draw(allShader);
		myCar3.draw(allShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Pass 'Car &car' so we can modify the real car object
// Add a global or pass-by-reference variable to track key press to prevent rapid toggling
bool enterKeyPressed = false;

void processInput(GLFWwindow *window, Car &car, Camera &camera, bool &inCar, float deltaTime)
{
	// Exit Game
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// --- ENTER / EXIT CAR LOGIC ---
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		if (!enterKeyPressed) // Only trigger once per press
		{
			float dist = glm::distance(camera.Position, car.position);

			if (inCar)
			{
				// EXIT: always allowed if inside
				inCar = false;
				// Teleport player slightly to the left of the car so they don't get stuck inside
				camera.Position = car.position + glm::vec3(-2.0f, 0.0f, 0.0f);
				camera.Position.y = 3.7f; // Reset height to standing
			}
			else if (dist < 3.0f)
			{
				// ENTER: Only allowed if close enough (3.0 units)
				inCar = true;
			}
			enterKeyPressed = true;
		}
	}
	else
	{
		enterKeyPressed = false; // Reset when key is released
	}

	// --- MOVEMENT LOGIC ---
	if (inCar)
	{
		// DRIVING MODE (Arrow Keys)
		// ... (Your existing Car Control Logic) ...
		float carSpeed = 5.0f * deltaTime;
		float turnSpeed = 60.0f * deltaTime;
		float rad = glm::radians(car.rotationAngle);
		glm::vec3 forwardDir = glm::vec3(sin(rad), 0.0f, cos(rad));

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			car.move(-forwardDir * carSpeed);
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				car.rotate(turnSpeed);
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				car.rotate(-turnSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			car.move(forwardDir * carSpeed);
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				car.rotate(-turnSpeed);
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				car.rotate(turnSpeed);
		}
	}
	else
	{
		// WALKING MODE (WASD)
		// Standard Camera Controls
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);

		// Keep camera at eye level (optional, simple gravity)
		// camera.Position.y = 1.7f;
	}

	// --- DOOR ANIMATION LOGIC ---
// Define a speed for the animation (e.g., 2.0f means it takes 0.5 seconds to open)
float doorSpeed = 2.0f * deltaTime; 
if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    car.setDoorOpenAngle(car.getDoorOpenAngle() + 2.0f * deltaTime);

if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    car.setDoorOpenAngle(car.getDoorOpenAngle() - 2.0f * deltaTime);

}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
