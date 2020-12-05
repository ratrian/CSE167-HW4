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
	glm::vec3 difference;
	difference.x = bs.getCenter().x - center.x;
	difference.y = bs.getCenter().y - center.y;
	difference.z = bs.getCenter().z - center.z;
	
	GLfloat dist = sqrt(difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
	if (dist < bs.getRadius() + radius)
		return true;
	return false;
}