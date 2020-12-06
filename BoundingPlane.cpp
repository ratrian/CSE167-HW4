#include "BoundingPlane.h"

BoundingPlane::BoundingPlane(glm::vec3 n, glm::vec3 p)
{
	BoundingPlane::n = n;
	BoundingPlane::p = p;
}

glm::vec3 BoundingPlane::getN()
{
	return n;
}

glm::vec3 BoundingPlane::getP()
{
	return p;
}

bool BoundingPlane::collide(BoundingSphere bs)
{
	GLfloat dist = glm::dot(bs.getCenter(), n) - glm::dot(p, n);
	std::cout << "dist: " << dist << std::endl;
	if (dist <= bs.getRadius())
		return true;
	return false;
}