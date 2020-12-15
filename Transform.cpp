#include "Transform.h"

Transform::Transform()
{
	M = glm::mat4(1.0);
	mT = glm::mat4(1.0);
	mR = glm::mat4(1.0);
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::removeChild(unsigned idx)
{
	std::list<Node*>::iterator it = children.begin();
	advance(it, idx);
	children.erase(it);
}

void Transform::draw(GLuint shaderProgram, GLuint particleShaderProgram, glm::mat4 C, GLfloat currTime)
{
	std::list<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		(*it)->draw(shaderProgram, particleShaderProgram, M * C * mT * mR, currTime);
	}
}

void Transform::update()
{
	rotate(0.0002f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Transform::translate(glm::vec3 t)
{
	mT = glm::translate(glm::mat4(1.0), t) * mT;
}

void Transform::rotate(float rotAngle, glm::vec3 rotAxis)
{
	mR = glm::rotate(glm::mat4(1.0f), glm::degrees(rotAngle), rotAxis) * mR;
}