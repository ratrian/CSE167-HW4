#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class ParticleSystem
{
private:
	struct Particle {
		glm::vec3 position, velocity;
		float life;
	};

	static const int MAX_PARTICLES = 200;
	static const int PARTICLE_LIFE = 3;

	std::vector<glm::vec3> positionData;
	bool appear;

	Particle particles[MAX_PARTICLES]; // Simulated data

	GLuint VAO, VBO;

public:
	ParticleSystem(glm::vec3 geometryPositionData[], bool appear);
	~ParticleSystem();

	void draw(GLuint shaderProgram, glm::mat4 model, GLfloat timePassed);
	void update(float deltaTime);
};

#endif