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

bool BoundingSphere::collide(BoundingSphere boundingSphere)
{
	glm::vec3 difference;
	difference.x = boundingSphere.getCenter().x - center.x;
	difference.y = boundingSphere.getCenter().y - center.y;
	difference.z = boundingSphere.getCenter().z - center.z;
	
	GLfloat dist = sqrt(difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
	std::cout << "dist: " << dist << ", " << "sum radii: " << boundingSphere.getRadius() + radius << std::endl;
	if (dist < boundingSphere.getRadius() + radius)
		return true;
	return false;
}