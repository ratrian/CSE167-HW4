#include "Geometry.h"
#include "stb_image.h"

Geometry::Geometry(std::string objFilename, GLfloat scaleFactor, GLfloat drawAstro, Material* material)
	: drawAstro(drawAstro), material(material)
{
	/*
	 * TODO: Section 2: Currently, all the points are hard coded below.
	 * Modify this to read faces from an obj file.
	 */

	std::vector<glm::vec3> inputPoints;
	std::vector<glm::vec2> inputTextures;
	std::vector<glm::vec3> inputNormals;
	std::vector<glm::vec3> pointIndices;
	std::vector<glm::vec3> textureIndices;
	std::vector<glm::vec3> normalIndices;

	std::ifstream objFile(objFilename); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point.
				inputPoints.push_back(point);
			}
			// If the line is about vertex texture (starting with a "vt").
			else if (label == "vt")
			{
				// Read the later three float numbers and use them as the coordinates.
				glm::vec2 texture;
				ss >> texture.x >> texture.y;

				// Process the texture.
				inputTextures.push_back(texture);
			}
			// If the line is about vertex normal (starting with a "vn").
			else if (label == "vn")
			{
				// Read the later three float numbers and use them as the coordinates.
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;

				// Process the normal.
				inputNormals.push_back(normal);
			}
			// If the line is about face (starting with a "f").
			else if (label == "f")
			{
				// Read the later three integers and use them as the indices.			
				string one, two, three;
				ss >> one >> two >> three;

				glm::ivec3 pIdx, tIdx, nIdx;
				pIdx.x = stoi(one.substr(0, one.find("/"))) - 1;
				one = one.substr(one.find("/") + 1);
				tIdx.x = stoi(one.substr(0, one.find("/"))) - 1;
				one = one.substr(one.find("/") + 1);
				nIdx.x = stoi(one) - 1;
				pIdx.y = stoi(two.substr(0, two.find("/"))) - 1;
				two = two.substr(two.find("/") + 1);
				tIdx.y = stoi(two.substr(0, two.find("/"))) - 1;
				two = two.substr(two.find("/") + 1);
				nIdx.y = stoi(two) - 1;
				pIdx.z = stoi(three.substr(0, three.find("/"))) - 1;
				three = three.substr(three.find("/") + 1);
				tIdx.z = stoi(three.substr(0, three.find("/"))) - 1;
				three = three.substr(three.find("/") + 1);
				nIdx.z = stoi(three) - 1;

				// Process the index.
				pointIndices.push_back(pIdx);
				textureIndices.push_back(tIdx);
				normalIndices.push_back(nIdx);
			}
		}
	}
	else
		std::cerr << "Can't open the file " << objFilename << std::endl;

	objFile.close();

	model = glm::mat4(scaleFactor);

	unsigned j = 0;
	unsigned numIndices = pointIndices.size();
	for (unsigned i = 0; i < numIndices; i++) {
		points.push_back(inputPoints[pointIndices[i].x]);
		textures.push_back(inputTextures[textureIndices[i].x]);
		normals.push_back(inputNormals[normalIndices[i].x]);

		points.push_back(inputPoints[pointIndices[i].y]);
		textures.push_back(inputTextures[textureIndices[i].y]);
		normals.push_back(inputNormals[normalIndices[i].y]);

		points.push_back(inputPoints[pointIndices[i].z]);
		textures.push_back(inputTextures[textureIndices[i].z]);
		normals.push_back(inputNormals[normalIndices[i].z]);

		indices.push_back(glm::vec3(j, j + 1, j + 2));
		j += 3;
	}

	GLfloat minX = points[0].x;
	GLfloat maxX = points[0].x;
	GLfloat minY = points[0].y;
	GLfloat maxY = points[0].y;
	GLfloat minZ = points[0].z;
	GLfloat maxZ = points[0].z;

	int numPoints = points.size();

	for (int i = 0; i < numPoints; i++) {
		if (minX > points[i].x)
			minX = points[i].x;
		if (maxX < points[i].x)
			maxX = points[i].x;
		if (minY > points[i].y)
			minY = points[i].y;
		if (maxY < points[i].y)
			maxY = points[i].y;
		if (minZ > points[i].z)
			minZ = points[i].z;
		if (maxZ < points[i].z)
			maxZ = points[i].z;
	}

	GLfloat centX = (minX + maxX) / 2;
	GLfloat centY = (minY + maxY) / 2;
	GLfloat centZ = (minZ + maxZ) / 2;

	for (int i = 0; i < numPoints; i++) {
		points[i].x -= centX;
		points[i].y -= centY;
		points[i].z -= centZ;
	}

	GLfloat maxDist = sqrt((points[0].x) * (points[0].x) + (points[0].y) * (points[0].y) + (points[0].z) * (points[0].z));
	for (int i = 0; i < numPoints; i++) {
		if (maxDist < sqrt((points[i].x) * (points[i].x) + (points[i].y) * (points[i].y) + (points[i].z) * (points[i].z)))
			maxDist = sqrt((points[i].x) * (points[i].x) + (points[i].y) * (points[i].y) + (points[i].z) * (points[i].z));
	}

	boundingSphere = new BoundingSphere(glm::vec3(centX, centY, centZ), 0.8f * maxDist);

	appearanceEffect = new ParticleSystem(points, true);
	appearanceEffect = new ParticleSystem(points, false);

	// Generate a Vertex Array (VAO) and bind to it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate a Vertex Buffer Object (VBO) and bind to it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), textures.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (objFilename.compare("amongus_lobby.obj") == 0) {
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		string textureFilename = "amongus_lobby.png";
		int width, height, nrChannels;
		unsigned char* data;
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(textureFilename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << textureFilename << std::endl;
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		lobbyTexture = textureID;
	}
}

Geometry::~Geometry() 
{
	// Delete the VBO/NBO/EBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	delete boundingSphere;
	delete appearanceEffect;
	delete disappearanceEffect;
}

void Geometry::draw(GLuint shaderProgram, GLuint particleShaderProgram, glm::mat4 C, GLfloat currTime)
{
	glm::mat4 currModel = C * model;

	// Actiavte the shader program 
	glUseProgram(shaderProgram);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(currModel));
	glUniform1f(glGetUniformLocation(shaderProgram, "drawAstro"), drawAstro);
	glUniform1f(glGetUniformLocation(shaderProgram, "drawDiscoball"), 0.0);
	glUniform1f(glGetUniformLocation(shaderProgram, "drawLightSource"), 0.0);
	glUniform1f(glGetUniformLocation(shaderProgram, "drawAstro"), drawAstro);
	material->sendMatToShader(shaderProgram);

	// Bind the VAO
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lobbyTexture);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, sizeof(glm::vec3) * points.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);

	if (drawAstro == 1.0)
	{
		//appearanceEffect->draw(particleShaderProgram, currModel, currTime - appearanceTime);
		//disappearanceEffect->draw(particleShaderProgram, currModel, currTime - disappearanceTime);
	}
}

void Geometry::update()
{

}

void Geometry::setAppearanceTime(GLfloat appearanceTime)
{
	Geometry::appearanceTime = appearanceTime;
}

void Geometry::setDisappearanceTime(GLfloat disaappearanceTime)
{
	Geometry::disappearanceTime = disaappearanceTime;
}

void Geometry::updateBoundingSphere(BoundingSphere* boundingSphere)
{
	delete Geometry::boundingSphere;
	Geometry::boundingSphere = boundingSphere;
}

BoundingSphere* Geometry::getBoundingSphere()
{
	return boundingSphere;
}