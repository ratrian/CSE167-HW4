#ifndef _CUBE_H_
#define _CUBE_H_

#include "Object.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Cube : public Object
{
private:
	GLuint VAO, VBO, EBO, cubemapTexture;

public:
	Cube(float size);
	~Cube();

	unsigned int loadCubemap(std::vector<std::string> faces);
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
	void update();
	void spin(float deg);
};

#endif