#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include "Material.h"
#include "BoundingSphere.h"
#include "ParticleSystem.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

class Geometry : public Node
{
private:
	glm::mat4 model;

	std::vector<glm::vec3> points;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> indices;

	GLuint VAO, VBO, TBO, NBO, EBO, lobbyTexture;
	
	GLfloat appearanceTime, drawAstro;
	
	Material* material;

	BoundingSphere* boundingSphere;

	ParticleSystem* appearanceEffect;
	ParticleSystem* disappearanceEffect;

public:
	Geometry(std::string objFilename, GLfloat scaleFactor, GLfloat drawAstro, Material* material);
	~Geometry();
	
	void draw(GLuint shaderProgram, GLuint particleShaderProgram, glm::mat4 C, GLfloat currTime);
	void update();

	void setAppearanceTime(GLfloat appearanceTime);
	void updateBoundingSphere(BoundingSphere* boundingSphere);
	BoundingSphere* getBoundingSphere();
};

#endif