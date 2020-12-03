#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

bool Window::activated = false;
bool Window::actionLobby = true;
bool Window::actionLightSource = false;
glm::vec3 Window::lastPoint;

bool Window::carouselView = false;

bool Window::rotateCarousel = false;
bool Window::rotatePole = false;
bool Window::rotateCar = false;

PointLight* Window::pointLight;
LightSource* Window::lightSource;

Material* lobbyMaterial;
Geometry* Window::lobby;

Transform* Window::world;

Cube* Window::skybox;
Sphere* Window::discoball;

GLfloat Window::normalColoring = 0.0;
GLfloat pointSize;

// Camera Matrices
// Projection matrix:
glm::mat4 Window::projection;
// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

glm::vec3 Window::carouselPos(0, -6.6, 0);
glm::vec3 Window::carouselLookAtPoint(0, 0, 20);

// Shader Program ID
GLuint Window::shaderProgram;

// Skybox Shader Program ID
GLuint Window::skyboxShaderProgram;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Create a skybox shader program with a vertex shader and a fragment shader.
	skyboxShaderProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");

	// Check the shader program.
	if (!skyboxShaderProgram)
	{
		std::cerr << "Failed to initialize skybox shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	pointSize = 30.0;
	
	pointLight = new PointLight(glm::vec3(-11.0, 3.0, 0.0), glm::vec3(0.9, 0.9, 0.9), glm::vec3(-0.05, 0.9, 0.0));
	lightSource = new LightSource("sphere.obj", pointLight);

	// Set up lobby.
	world = new Transform();
	world->rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	lobbyMaterial = new Material(glm::vec3(0.1745, 0.01175, 0.01175), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	lobby = new Geometry("amongus_lobby.obj", 0.3f, pointSize, normalColoring, lobbyMaterial);
	world->addChild(lobby);

	skybox = new Cube(1000);
	discoball = new Sphere(eyePos);

	return true;
}

void Window::cleanUp()
{
	delete pointLight;
	delete lightSource;

	delete lobbyMaterial;
	delete lobby;

	delete world;

	delete skybox;
	delete discoball;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here
	discoball->update();
	if (rotateCarousel) {
		world->update();
		carouselPos = glm::vec3(glm::rotate(glm::mat4(1.0f), 0.0002f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(carouselPos, 1));
		carouselLookAtPoint = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::degrees(0.0002f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(carouselLookAtPoint, 1));
		if (carouselView)
			view = glm::lookAt(carouselPos, carouselLookAtPoint, upVector);
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	lightSource->draw(shaderProgram, glm::mat4(1.0));
	glUseProgram(0);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	world->draw(shaderProgram, glm::mat4(1.0));
	glUseProgram(0);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_L:
			actionLobby = !actionLobby;
			actionLightSource = !actionLightSource;
			break;
		case GLFW_KEY_V:
			carouselView = !carouselView;
			if (carouselView)
			{
				actionLobby = false;
				view = glm::lookAt(carouselPos, carouselLookAtPoint, upVector);
			}
			else
			{
				actionLobby = true;
				view = glm::lookAt(eyePos, lookAtPoint, upVector);
			}
			break;
		case GLFW_KEY_1:
			rotateCarousel = !rotateCarousel;
			break;
		case GLFW_KEY_2:
			rotatePole = !rotatePole;
			break;
		case GLFW_KEY_3:
			rotateCar = !rotateCar;
			break;

		default:
			break;
		}
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		activated = true;

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		lastPoint = trackBallMapping(xPos, yPos);

		glMatrixMode(GL_MODELVIEW);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		activated = false;
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (activated)
	{
		glm::vec3 currPoint = trackBallMapping(xPos, yPos);
		glm::vec3 direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001)
		{
			if (actionLobby)
			{
				eyePos.y += direction.y;
				eyePos.z += direction.x;
				lookAtPoint.y += direction.y;
				lookAtPoint.z += direction.x;
				view = glm::lookAt(eyePos, lookAtPoint, upVector);
			}
			if (actionLightSource)
			{
				float rotAngle = velocity * 0.05;
				glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
				lightSource->orbit(direction, rotAngle, rotAxis);
			}
		}
		lastPoint = currPoint;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	glMatrixMode(GL_PROJECTION);
	if (actionLobby)
	{
		glm::mat4 mT = glm::translate(glm::mat4(1.0), -eyePos);
		glm::mat4 mR = glm::rotate(glm::mat4(1.0), glm::degrees(GLfloat(yOffset * 0.001)), glm::vec3(0.0, 1.0, 0.0));
		lookAtPoint = glm::vec3(inverse(mT) * mR * mT * glm::vec4(lookAtPoint, 1.0));
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
	}
	if (actionLightSource)
		lightSource->translate(glm::vec3(yOffset * 0.01));
}

glm::vec3 Window::trackBallMapping(double xPos, double yPos)
{
	glm::vec3 v;
	float d;
	v.x = (2.0 * xPos - height) / height;
	v.y = (width - 2.0 * yPos) / width;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	glm::normalize(v);
	return v;
}
