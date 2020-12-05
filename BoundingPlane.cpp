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
	return false;
}