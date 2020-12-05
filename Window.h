#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "DirLight.h"
#include "LightSource.h"
#include "Transform.h"
#include "Geometry.h"
#include "BoundingPlane.h"
#include "Cube.h"
#include "Sphere.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	static bool activated, actionLobby, actionLightSource;
	static glm::vec3 lastPoint;

	static DirLight* dirLight;
	static PointLight* pointLight;
	static LightSource* lightSource;

	static Transform* lobbyTransform;
	static Transform* astroStillTransform[10];
	static Transform* astroMoving1Transform[10];
	static Transform* astroMoving2Transform[10];

	static Geometry* lobby;
	static Geometry* astroStill[10];
	static Geometry* astroMoving1[10];
	static Geometry* astroMoving2[10];

	static BoundingSphere* boxBoundingSphere[2];
	static BoundingPlane* wallBoundingPlane[6];

	static Cube* skybox;
	static Sphere* discoball;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, carouselPos, carouselLookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;
	static GLuint skyboxShaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	static glm::vec3 trackBallMapping(double xPos, double yPos);
};

#endif
