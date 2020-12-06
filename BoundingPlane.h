#ifndef _BOUNDING_PLANE_H_
#define _BOUNDING_PLANE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BoundingSphere.h"

class BoundingPlane
{
private:
	glm::vec3 n, p;

public:
	BoundingPlane(glm::vec3 n, glm::vec3 p);
		
	glm::vec3 getN();
	glm::vec3 getP();
	bool collide(BoundingSphere boundingSphere);
};

#endif
