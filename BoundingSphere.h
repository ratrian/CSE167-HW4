#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class BoundingSphere
{
private:
	glm::vec3 center;
	GLfloat radius;

public:
	BoundingSphere(glm::vec3 center, GLfloat radius);

	glm::vec3 getCenter();
	GLfloat getRadius();
	bool collide(BoundingSphere boundingSphere);
};

#endif