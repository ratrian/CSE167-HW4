#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

bool Window::activated = false;
bool Window::actionLobby = true;
bool Window::actionLightSource = false;
glm::vec3 Window::lastPoint;

DirLight* Window::dirLight;
PointLight* Window::pointLight;
LightSource* Window::lightSource;

Transform* Window::lobbyTransform;
Transform* Window::astroStillTransform[10];
Transform* Window::astroMoving1Transform[10];
Transform* Window::astroMoving2Transform[10];
Transform* currAstroTransform[10];

Material* lobbyMaterial;
Geometry* Window::lobby;

Material* astroMaterial[10];
Geometry* Window::astroStill[10];
Geometry* Window::astroMoving1[10];
Geometry* Window::astroMoving2[10];
Geometry* currAstro[10];

BoundingSphere* Window::boxBoundingSphere[2];
BoundingPlane* Window::wallBoundingPlane[6];

Cube* Window::skybox;
Sphere* Window::discoball;

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
	
	dirLight = new DirLight(glm::vec3(0.0, -3.0, -6.0), glm::vec3(0.7, 0.7, 0.7));
	pointLight = new PointLight(glm::vec3(-3.0, -5.0, -6.0), glm::vec3(0.7, 0.7, 0.7), glm::vec3(-0.05, 0.9, 0.0));
	lightSource = new LightSource("sphere.obj", pointLight);

	// Set up lobby.
	lobbyTransform = new Transform();
	lobbyTransform->rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	lobbyMaterial = new Material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	lobby = new Geometry("amongus_lobby.obj", 0.5f, pointSize, 0.0f, lobbyMaterial);
	lobbyTransform->addChild(lobby);

	// Set up astronauts.
	for (unsigned i = 0; i < 10; i++) {
		astroStillTransform[i] = new Transform();
		astroStillTransform[i]->translate(glm::vec3(-13.0f + float(i * 3), -3.0f, 0.0f));

		astroMoving1Transform[i] = new Transform();
		astroMoving1Transform[i]->translate(glm::vec3(-13.0f + float(i * 3), -3.0f, 0.0f));

		astroMoving2Transform[i] = new Transform();
		astroMoving2Transform[i]->translate(glm::vec3(-13.0f + float(i * 3), -3.0f, 0.0f));

		currAstroTransform[i] = astroMoving1Transform[i];
	}
	astroMaterial[0] = new Material(glm::vec3(62.0f/255.0f, 71.0f/255.0f, 78.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[1] = new Material(glm::vec3(19.0f/255.0f, 46.0f/255.0f, 209.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[2] = new Material(glm::vec3(113.0f/255.0f, 73.0f/255.0f, 29.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[3] = new Material(glm::vec3(78.0f/255.0f, 239.0f/255.0f, 56.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[4] = new Material(glm::vec3(241.0f/255.0f, 125.0f/255.0f, 12.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[5] = new Material(glm::vec3(236.0f/255.0f, 84.0f/255.0f, 187.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[6] = new Material(glm::vec3(108.0f/255.0f, 47.0f/255.0f, 188.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[7] = new Material(glm::vec3(197.0f/255.0f, 18.0f/255.0f, 17.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[8] = new Material(glm::vec3(214.0f/255.0f, 223.0f/255.0f, 241.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	astroMaterial[9] = new Material(glm::vec3(246.0f/255.0f, 246.0f/255.0f, 87.0f/255.0f), glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 0.6);
	for (unsigned i = 0; i < 10; i++) {
		astroStill[i] = new Geometry("amongus_astro_still.obj", 0.5f, pointSize, 1.0f, astroMaterial[i]);
		astroStill[i]->updateBoundingSphere(new BoundingSphere(glm::vec3(astroStill[i]->getBoundingSphere()->getCenter().x - 13.0f + float(i * 3), astroStill[i]->getBoundingSphere()->getCenter().y - 3.0f, astroStill[i]->getBoundingSphere()->getCenter().z), astroStill[i]->getBoundingSphere()->getRadius()));
		astroStillTransform[i]->addChild(astroStill[i]);

		astroMoving1[i] = new Geometry("amongus_astro_moving1.obj", 0.5f, pointSize, 1.0f, astroMaterial[i]);
		astroMoving1[i]->updateBoundingSphere(new BoundingSphere(glm::vec3(astroMoving1[i]->getBoundingSphere()->getCenter().x - 13.0f + float(i * 3), astroMoving1[i]->getBoundingSphere()->getCenter().y - 3.0f, astroMoving1[i]->getBoundingSphere()->getCenter().z), astroMoving1[i]->getBoundingSphere()->getRadius()));
		astroMoving1Transform[i]->addChild(astroMoving1[i]);

		astroMoving2[i] = new Geometry("amongus_astro_moving2.obj", 0.5f, pointSize, 1.0f, astroMaterial[i]);
		astroMoving2[i]->updateBoundingSphere(new BoundingSphere(glm::vec3(astroMoving2[i]->getBoundingSphere()->getCenter().x - 13.0f + float(i * 3), astroMoving2[i]->getBoundingSphere()->getCenter().y - 3.0f, astroMoving2[i]->getBoundingSphere()->getCenter().z), astroMoving2[i]->getBoundingSphere()->getRadius()));
		astroMoving2Transform[i]->addChild(astroMoving2[i]);

		currAstro[i] = astroMoving1[i];
		lobbyTransform->addChild(currAstroTransform[i]);
	}
/*
	BoundingSphere[0] = new BoundingSphere(glm::vec3(), );
	BoundingSphere[1] = new BoundingSphere(glm::vec3(), );

	BoundingPlane[0] = new BoundingPlane(glm::vec3(0.0, -1.0, 0.0), glm::vec3());
	BoundingPlane[1] = new BoundingPlane(glm::vec3(-1.0, 0.0, 0.0), glm::vec3());
	BoundingPlane[2] = new BoundingPlane(glm::vec3(-sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0.0), glm::vec3());
	BoundingPlane[3] = new BoundingPlane(glm::vec3(0.0, 1.0, 0.0), glm::vec3());
	BoundingPlane[4] = new BoundingPlane(glm::vec3(sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0.0), glm::vec3());
	BoundingPlane[5] = new BoundingPlane(glm::vec3(1.0, 0.0, 0.0), glm::vec3());
*/
	skybox = new Cube(1000);
	discoball = new Sphere(eyePos);

	return true;
}

void Window::cleanUp()
{
	delete dirLight;
	delete pointLight;
	delete lightSource;

	delete lobbyTransform;

	for (unsigned i = 0; i < 10; i++) {
		delete astroStillTransform[i];
		delete astroMoving1Transform[i];
		delete astroMoving2Transform[i];
	}

	delete lobbyMaterial;
	delete lobby;

	for (unsigned i = 0; i < 10; i++) {
		delete astroMaterial[i];
		delete astroStill[i];
		delete astroMoving1[i];
		delete astroMoving2[i];
	}

	for (unsigned i = 0; i < 2; i++) {
		delete boxBoundingSphere[i];
	}
	for (unsigned i = 0; i < 6; i++) {
		delete wallBoundingPlane[i];
	}

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
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dirLight->sendLightToShader(shaderProgram);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	lobbyTransform->draw(shaderProgram, glm::mat4(1.0));
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
		bool collision = false;

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
		case GLFW_KEY_W:
			/*for (unsigned i = 0; i < 2; i++) {
				if (boxBoundingSphere[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}
			for (unsigned i = 0; i < 6; i++) {
				if (wallBoundingPlane[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}*/
			collision = false;
			for (unsigned i = 1; i < 10; i++) {
				if (currAstro[i]->getBoundingSphere()->collide(*currAstro[0]->getBoundingSphere()))
				{
					collision = true;
					break;
				}
			}
			if (collision)
				break;
			astroStillTransform[0]->translate(glm::vec3(0.0, 0.0, -1.0));
			astroMoving1Transform[0]->translate(glm::vec3(0.0, 0.0, -1.0));
			astroMoving2Transform[0]->translate(glm::vec3(0.0, 0.0, -1.0));
			currAstroTransform[0] = astroMoving1Transform[0];
			currAstro[0]->updateBoundingSphere(new BoundingSphere(glm::vec3(currAstro[0]->getBoundingSphere()->getCenter().x, currAstro[0]->getBoundingSphere()->getCenter().y, currAstro[0]->getBoundingSphere()->getCenter().z - 1.0f), currAstro[0]->getBoundingSphere()->getRadius()));
			break;
		case GLFW_KEY_A:
			/*for (unsigned i = 0; i < 2; i++) {
				if (boxBoundingSphere[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}
			for (unsigned i = 0; i < 6; i++) {
				if (wallBoundingPlane[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}*/
			collision = false;
			for (unsigned i = 1; i < 10; i++) {
				if (currAstro[i]->getBoundingSphere()->collide(*currAstro[0]->getBoundingSphere()))
				{
					collision = true;
					break;
				}
			}
			if (collision)
				break;
			astroStillTransform[0]->translate(glm::vec3(-1.0, 0.0, 0.0));
			astroMoving1Transform[0]->translate(glm::vec3(-1.0, 0.0, 0.0));
			astroMoving2Transform[0]->translate(glm::vec3(-1.0, 0.0, 0.0));
			currAstroTransform[0] = astroMoving1Transform[0];
			currAstro[0]->updateBoundingSphere(new BoundingSphere(glm::vec3(currAstro[0]->getBoundingSphere()->getCenter().x - 1.0f, currAstro[0]->getBoundingSphere()->getCenter().y, currAstro[0]->getBoundingSphere()->getCenter().z), currAstro[0]->getBoundingSphere()->getRadius()));
			break;
		case GLFW_KEY_S:
			/*for (unsigned i = 0; i < 2; i++) {
				if (boxBoundingSphere[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}
			for (unsigned i = 0; i < 6; i++) {
				if (wallBoundingPlane[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}*/
			collision = false;
			for (unsigned i = 1; i < 10; i++) {
				if (currAstro[i]->getBoundingSphere()->collide(*currAstro[0]->getBoundingSphere()))
				{
					collision = true;
					break;
				}
			}
			if (collision)
				break;
			astroStillTransform[0]->translate(glm::vec3(0.0, 0.0, 1.0));
			astroMoving1Transform[0]->translate(glm::vec3(0.0, 0.0, 1.0));
			astroMoving2Transform[0]->translate(glm::vec3(0.0, 0.0, 1.0));
			currAstroTransform[0] = astroMoving1Transform[0];
			currAstro[0]->updateBoundingSphere(new BoundingSphere(glm::vec3(currAstro[0]->getBoundingSphere()->getCenter().x, currAstro[0]->getBoundingSphere()->getCenter().y, currAstro[0]->getBoundingSphere()->getCenter().z + 1.0f), currAstro[0]->getBoundingSphere()->getRadius()));
			break;
		case GLFW_KEY_D:
			/*for (unsigned i = 0; i < 2; i++) {
				if (boxBoundingSphere[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}
			for (unsigned i = 0; i < 6; i++) {
				if (wallBoundingPlane[i]->collide(*currAstro[0]->getBoundingSphere()))
					break;
			}*/
			collision = false;
			for (unsigned i = 1; i < 10; i++) {
				if (currAstro[i]->getBoundingSphere()->collide(*currAstro[0]->getBoundingSphere()))
				{
					collision = true;
					break;
				}
			}
			if (collision)
				break;
			astroStillTransform[0]->translate(glm::vec3(1.0, 0.0, 0.0));
			astroMoving1Transform[0]->translate(glm::vec3(1.0, 0.0, 0.0));
			astroMoving2Transform[0]->translate(glm::vec3(1.0, 0.0, 0.0));
			currAstroTransform[0] = astroMoving1Transform[0];
			currAstro[0]->updateBoundingSphere(new BoundingSphere(glm::vec3(currAstro[0]->getBoundingSphere()->getCenter().x + 1.0f, currAstro[0]->getBoundingSphere()->getCenter().y, currAstro[0]->getBoundingSphere()->getCenter().z), currAstro[0]->getBoundingSphere()->getRadius()));
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
			float rotAngle = velocity * 0.05;
			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			if (actionLobby)
				lobbyTransform->rotate(rotAngle, rotAxis);
			if (actionLightSource)
				lightSource->orbit(direction, rotAngle, rotAxis);
		}
		lastPoint = currPoint;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	glMatrixMode(GL_PROJECTION);
	if (actionLobby)
	{
		eyePos.z -= yOffset * 0.1;
		lookAtPoint.z -= yOffset * 0.1;
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
