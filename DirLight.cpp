#include "DirLight.h"

DirLight::DirLight(glm::vec3 dir, glm::vec3 color)
{
	DirLight::dir = dir;
	DirLight::color = color;
}

void DirLight::sendLightToShader(GLuint shader)
{
	glUseProgram(shader);
	glUniform3fv(glGetUniformLocation(shader, "lightDir"), 1, glm::value_ptr(dir));
	glUniform3fv(glGetUniformLocation(shader, "lightCol"), 1, glm::value_ptr(color));
	glUseProgram(0);
}