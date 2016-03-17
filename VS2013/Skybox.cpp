#include "Skybox.h"

vector<glm::vec3> Skybox::cubeV;
vector<GLuint> Skybox::cubeInd;
vector<const char*> Skybox::filepath;
Shader* Skybox::skyShaderptr = NULL;

Skybox::Skybox(GLuint size)
{
	this->size = size;

	cubeV = {         
		glm::vec3(-2.0f* size, 2.0f* size, -2.0f* size),
		glm::vec3(-2.0f* size, -2.0f* size, -2.0f* size),
		glm::vec3(2.0f*size, 2.0f*size, -2.0f*size),
		glm::vec3(2.0f*size, -2.0f*size, -2.0f*size),
		
		glm::vec3(-2.0f*size, -2.0f*size, 2.0f*size),
		glm::vec3(-2.0f*size, -2.0f*size, -2.0f*size),
		glm::vec3(-2.0f*size, 2.0f*size, 2.0f*size),
		glm::vec3(-2.0f*size, 2.0f*size, -2.0f*size),
	
		glm::vec3(2.0f*size, -2.0f*size, -2.0f*size),
		glm::vec3(2.0f*size, -2.0f*size, 2.0f*size),
		glm::vec3(2.0f*size, 2.0f*size, -2.0f*size),
		glm::vec3(2.0f*size, 2.0f*size, 2.0f*size),
	
		glm::vec3(-2.0f*size, -2.0f*size, 2.0f*size),
		glm::vec3(-2.0f*size, 2.0f*size, 2.0f*size),
		glm::vec3(2.0f*size, -2.0f*size, 2.0f*size),
		glm::vec3(2.0f*size, 2.0f*size, 2.0f*size),

		glm::vec3(-2.0f*size, 2.0f*size, -2.0f*size),
		glm::vec3(2.0f*size, 2.0f*size, -2.0f*size),
		glm::vec3(-2.0f*size, 2.0f*size, 2.0f*size),
		glm::vec3(2.0f*size, 2.0f*size, 2.0f*size),

		glm::vec3(-2.0f*size, -2.0f*size, -2.0f*size),
		glm::vec3(-2.0f*size, -2.0f*size, 2.0f*size),
		glm::vec3(-2.0f*size, -2.0f*size, 2.0f*size),
		glm::vec3(2.0f*size, -2.0f*size, -2.0f)
	};
	
	skyShaderptr = (new Shader("../Source/SKY_VERTEX_SHADER.vs", "../Source/SKY_FRAG_SHADER.frag"));
	shader_program = skyShaderptr->Program;

	loadIndices();
	createBuffers();
	loadTexture();
	
}

Skybox::~Skybox()
{
	cubeV.clear();
	cubeInd.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete skyShaderptr;
	skyShaderptr = nullptr;
	vector<const char*>().swap(filepath);
	filepath.clear();
}

void Skybox::loadTexture()
{
	filepath.push_back("../Images/right.jpg"); // Right
	filepath.push_back("../Images/left.jpg"); // Left
	filepath.push_back("../Images/top.jpg"); // Top
	filepath.push_back("../Images/bottom.jpg"); // Bottom
	filepath.push_back("../Images/back.jpg"); // Back
	filepath.push_back("../Images/front.jpg"); // Front

	createTexture();
}

void Skybox::createTexture()
{
	//skyShaderptr = (new Shader("../Source/SKY_VERTEX_SHADER.vs", "../Source/SKY_FRAG_SHADER.frag"));
	//shader_program = skyShaderptr->Program;

	int width, height;
	unsigned char* image;
	glBindTexture(GL_TEXTURE_CUBE_MAP, faceTex);

	for (int i = 0; i < filepath.size(); i++)
	{
		// Load image, create texture and generate mipmaps
		image = SOIL_load_image(filepath[i], &width, &height, 0, SOIL_LOAD_RGB);
		if (image == '\0')
		{
			std::cout << "Unable to load image. Break." << std::endl;
			break;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

}

void Skybox::createBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeV[0])* cubeV.size(), &cubeV[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd[0]) * cubeInd.size(), &cubeInd[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Skybox::draw()
{
	skyShaderptr->Use();
	glDepthMask(GL_FALSE);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, faceTex);
	glDrawElements(GL_TRIANGLES, cubeInd.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void Skybox::loadIndices()
{
	for (int i = 0; i < (cubeV.size() / 4); i++)
	{
		cubeInd.push_back(0 + (i * 4));
		cubeInd.push_back(1 + (i * 4));
		cubeInd.push_back(2 + (i * 4));
		cubeInd.push_back(2 + (i * 4));
		cubeInd.push_back(1 + (i * 4));
		cubeInd.push_back(3 + (i * 4));
	}
}

GLuint Skybox::getShaderProgram()
{
	return shader_program;
}