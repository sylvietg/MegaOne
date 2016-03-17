#include "Tree.h"

vector<GLfloat> Tree::treeV;
vector<GLuint> Tree::treeInd;
Shader* Tree::treeShaderptr = NULL;
const char* Tree::filepath = NULL;

Tree::Tree(GLfloat w, GLfloat h, const char* filepath)
{
	this->w = w;
	this->h = h;
	this->filepath = filepath;
	treeShaderptr = (new Shader("../Source/TREE_VERTEX_SHADER.vs", "../Source/TREE_FRAG_SHADER.frag"));
	shader_program = treeShaderptr->Program;
	
	defineVertices();
	
	treeInd = {
		0,1,2,
		2,1,3
	};

	createBuffers();
	loadTexture();

	
};

Tree::~Tree()
{
	treeV.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete treeShaderptr;
	treeShaderptr = nullptr;
	delete filepath;
	filepath = nullptr;
}

void Tree::writeColorCoord()
{
	treeV.push_back(0.0f);
	treeV.push_back(1.0f);
	treeV.push_back(0.0f);
}

void Tree::defineVertices()
{
	treeV.push_back(-(w / 2)); // Top left
	treeV.push_back(h);
	treeV.push_back(0.0f);

	writeColorCoord();	// Color coordinates

	treeV.push_back(0.0f);	// Texture coordinates
	treeV.push_back(1.0f);

	treeV.push_back(-(w / 2)); // Bottom left
	treeV.push_back(0.0f);
	treeV.push_back(0.0f);

	writeColorCoord();	// Color coordinates

	treeV.push_back(0.0f);	// Texture coordinates
	treeV.push_back(0.0f);

	treeV.push_back(w / 2); // Bottom right
	treeV.push_back(1.0f);
	treeV.push_back(0.0f);

	writeColorCoord();	// Color coordinates

	treeV.push_back(0.0f);	// Texture coordinates
	treeV.push_back(1.0f);

	treeV.push_back(w / 2); // Top right
	treeV.push_back(h);
	treeV.push_back(0.0f);	

	writeColorCoord();	// Color coordinates

	treeV.push_back(1.0f);	// Texture coordinates
	treeV.push_back(1.0f);
}

void Tree::createBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * treeV.size(), &treeV, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, treeInd.size(), &treeInd, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // Texture
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Tree::draw()
{
//	treeShaderptr->Use;
//	loadTexture();
//	createBuffers();
	glDepthMask(GL_FALSE);
	glBindVertexArray(VAO);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
//	glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);
	glDrawElements(GL_TRIANGLES, treeInd.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

GLuint Tree::getShaderProgram()
{
	return shader_program;
}

void Tree::loadTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == '\0')
	{
		std::cout << "Unable to load image." << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

}