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

class ParticleSystem
{
private:
	struct Particle {
		glm::vec3 position, velocity;
		float life;

		// Set initial position and velocity
		void initialize();

		// Run one time-stop
		void update(float deltaTime);
	};

	static const int MAX_PARTICLES = 200;
	static const int PARTICLE_LIFE = 3;

	GLuint VAO, VBO;

	Particle particles[MAX_PARTICLES]; // Simulated data
	glm::vec3 position_data[MAX_PARTICLES]; // VBO data

public:
	ParticleSystem();
	~ParticleSystem();

	void update(float deltaTime);
	void draw(GLuint shader, glm::mat4 model);
};

#endif