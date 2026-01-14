#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "tools/Polygon.h"
#include "tools/Cube.h"
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

vector<GLuint> loadTextures(vector<string> paths, GLuint wrapOption = GL_REPEAT, GLuint filterOption = GL_LINEAR)
{
	vector<GLuint> textures = {};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterOption);

	for (string path : paths)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		int width, height, nrChannels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
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
	Shader lightSourceShader("./shaders/vs/LightSource2.vs", "./shaders/fs/LightSource2.fs");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(true);

	vec3 pointLightPositions[] = {
		vec3(0.0f, 0.0f, 4.0f),
	};
	vec3 pointLightColors[] = {
		vec3(1.0f, 1.0f, 1.0f),
	};

	vector<string> texturePaths = {};
	texturePaths.push_back("./textures/window.png");
	texturePaths.push_back("./textures/white.png");
	vector<GLuint> textures = loadTextures(texturePaths);

	Model ball("./models/ball.glb");

	int numberOfBalls = 3;

	vector<vec3> ballsLocations = {};
	ballsLocations.push_back(vec3(-2.0f, -1.73f, -1.0f));
	ballsLocations.push_back(vec3(0.0f, 1.73f, -1.0f));
	ballsLocations.push_back(vec3(2.0f, -1.73f, -1.0f));

	vector<vec3> vertices = {};
	vertices.push_back(vec3(-4.0f, -3.46f, 1.0f));
	vertices.push_back(vec3(4.0f, -3.46f, 1.0f));
	vertices.push_back(vec3(4.0f, 3.46f, 1.0f));
	vertices.push_back(vec3(-4.0f, 3.46f, 1.0f));

	vector<vec2> texcoords = {};
	texcoords.push_back(vec2(0.0f, 0.0f));
	texcoords.push_back(vec2(1.0f, 0.0f));
	texcoords.push_back(vec2(1.0f, 1.0f));
	texcoords.push_back(vec2(0.0f, 1.0f));
	PolygonWithTransparency pane(vertices, vec4(0.69f, 0.72f, 0.73f, 0.2f));
	// PolygonWithTexture pane(vertices, texcoords, vec3(1.0f, 1.0f, 1.0f));
	// pane.setTexture(textures[0]);

	vector<Cube> lightSourceCubes = {};
	for (int i = 0; i < sizeof(pointLightColors) / sizeof(pointLightColors[0]); i++)
	{
		Cube LightSource(pointLightPositions[i], 0.2f, pointLightColors[i]);
		lightSourceCubes.push_back(LightSource);
	}

	while (!glfwWindowShouldClose(window))
	{
		allShader.use();
		allShader.setVec3("viewPos", camera.Position);

#pragma region Light Settings
		// Point light 1
		allShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		allShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
		allShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		allShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		allShader.setFloat("pointLights[0].constant", 1.0f);
		allShader.setFloat("pointLights[0].linear", 0.0f);
		allShader.setFloat("pointLights[0].quadratic", 0.0f);
#pragma endregion

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float time = (float)glfwGetTime();

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 projection = mat4(1.0f);
		projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		allShader.setMat4("projection", projection);

		mat4 view = mat4(1.0f);
		view = view = camera.GetViewMatrix();
		allShader.setMat4("view", view);

		glBindTexture(GL_TEXTURE_2D, textures[1]);
		allShader.setVec4("objectColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
		allShader.setMat4("model", translate(mat4(1.0f), ballsLocations[0]));
		allShader.setVec3("material.ambient", 0.25f, 0.20725f, 0.20725f);
		allShader.setVec3("material.diffuse", 1.0f, 0.829f, 0.829f);
		allShader.setVec3("material.specular", 0.296648f, 0.296648f, 0.296648f);
		allShader.setFloat("material.shininess", 11.26f);
		ball.Draw(allShader);

		allShader.setVec4("objectColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
		allShader.setMat4("model", translate(mat4(1.0f), ballsLocations[1]));
		allShader.setVec3("material.ambient", 0.25f, 0.25f, 0.25f);
		allShader.setVec3("material.diffuse", 0.4f, 0.4f, 0.4f);
		allShader.setVec3("material.specular", 0.774597f, 0.774597f, 0.774597f);
		allShader.setFloat("material.shininess", 76.8f);
		ball.Draw(allShader);

		allShader.setVec4("objectColor", vec4(1.0f, 1.0f, 1.0f, 1.0f));
		allShader.setMat4("model", translate(mat4(1.0f), ballsLocations[2]));
		allShader.setVec3("material.ambient", 0.0f, 0.0f, 0.0f);
		allShader.setVec3("material.diffuse", 0.5f, 0.5f, 0.0f);
		allShader.setVec3("material.specular", 0.6f, 0.6f, 0.5f);
		allShader.setFloat("material.shininess", 32.0f);
		ball.Draw(allShader);

		lightSourceShader.use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);

		glBindTexture(GL_TEXTURE_2D, textures[1]);
		mat4 model = glm::mat4(1.0f);
		for (unsigned int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			lightSourceShader.setMat4("model", model);
			lightSourceCubes[i].transformation(model);
			lightSourceCubes[i].draw(lightSourceShader);
		}

		allShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		allShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		allShader.setVec3("material.specular", 0.8f, 0.8f, 0.8f);
		allShader.setFloat("material.shininess", 0.0f);
		pane.draw(allShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
