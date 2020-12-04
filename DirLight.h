#ifndef _DIR_LIGHT_H_
#define _DIR_LIGHT_H_

#include "Object.h"

using namespace std;

class DirLight
{
private:
	glm::vec3 dir, color;

public:
	DirLight(glm::vec3 dir, glm::vec3 color);

	void sendLightToShader(GLuint shader);
};

#endif