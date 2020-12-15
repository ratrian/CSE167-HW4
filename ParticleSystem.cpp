#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(std::vector<glm::vec3> geometryPositionData, bool appear) : appear(appear)
{
	for (unsigned i = 0; i < MAX_PARTICLES; i++)
	{
		if (appear)
			positionData.push_back(geometryPositionData[i] + glm::vec3(0.01f * (rand() % 2), 2.0f, 0.01f * (rand() % 2)));
		else
			positionData.push_back(geometryPositionData[MAX_PARTICLES + i] - glm::vec3(0.01f * (rand() % 2), 2.0f, 0.01f * (rand() % 2)));

		particles[i].velocity = glm::vec3(0.1f);
		particles[i].life = PARTICLE_LIFE;
		srand(i);
	}

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positionData.size(), positionData.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the particle 


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::draw(GLuint shaderProgram, glm::mat4 model, GLfloat timePassed)
{
	if (timePassed > 0.0f && timePassed <= 3.0f)
	{
		// Actiavte the particle shader program 
		glUseProgram(shaderProgram);

		// Get the particle shader variable locations and send the uniform data to the shader 
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Bind the VAO
		glBindVertexArray(VAO);

		// Set point size
		glPointSize(1);

		// Draw the points 
		glDrawArrays(GL_POINTS, 0, positionData.size());

		// Unbind the VAO and particle shader program
		glBindVertexArray(0);

		// Unbind the shader program
		glUseProgram(0);
	}
}

void ParticleSystem::update(float deltaTime)
{
	for (unsigned i = 0; i < MAX_PARTICLES; i++)
	{
		if (particles[i].life < 0)
			particles[i].life = 0;
		else if (particles[i].life > 0)
		{
			particles[i].life -= deltaTime;
			if (appear) {
				positionData[i] -= deltaTime * particles[i].velocity;
			}
			else
				positionData[i] += deltaTime * particles[i].velocity;
		}
	}
}