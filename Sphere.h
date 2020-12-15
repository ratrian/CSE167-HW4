#ifndef _SPHERE_H_
#define _SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Node.h"

class Sphere : public Node {
	public:
		glm::mat4 model;
		glm::vec3 eyePos;

		GLuint vao = 0, vbo = 0, vbo_n = 0, ebo = 0;

		int stackCount = 40;
		int sectorCount = 40;
		int numsToDraw;

		Sphere(glm::vec3 eyePos);
		~Sphere();

		void draw(GLuint shaderProgram, GLuint particleShaderProgram, glm::mat4 C, GLfloat currTime);
		void update(float deltaTime);
		void spin(float deg);
};

#endif