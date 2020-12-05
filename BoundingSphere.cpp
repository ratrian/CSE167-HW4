#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(glm::vec3 center, GLfloat radius)
{
	BoundingSphere::center = center;
	BoundingSphere::radius = radius;
}

glm::vec3 BoundingSphere::getCenter()
{
	return center;
}

GLfloat BoundingSphere::getRadius()
{
	return radius;
}

bool BoundingSphere::collide(BoundingSphere bs)
{
	return false;
}