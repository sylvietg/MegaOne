#include "Block.h"

const char* Block::filepath1 = "c:/users/mimi/desktop/template/megaone/vs2013/road.png";
//const char* Block::filepath1 = "../ThirdParty/Simple OpenGL Image Library/test_rect.png";
const char* Block::filepath2 = "c:/users/mimi/desktop/template/megaone/vs2013/sidewalk.png";
vector<glm::vec3> Block::blockCoordinates;
vector<GLuint> Block::blockIndices;
Shader * Block::blockShaderptr = NULL;
GLuint Block::boardTexture;
GLuint Block::sidewalkGrassTexture;

Block::Block(){

	/*	NOTE
	*	The grid isn't used and isn't useful right now
	*	It could come in handy when we use those bumpmaps(?)
	*/
	GLfloat xDistance = 0.05f;
	GLfloat yDistance = 0.05f;
	GLfloat yDistanceCopy = 0.05f;
	GLuint counter = 0;
	glm::vec3 initialVector(-0.15f, -0.05f, 0.0f);
	for (GLuint i = 0; i < BLOCK_HEIGHT; i++){
		for (GLuint j = 0; j < BLOCK_WIDTH; j++){
			blockCoordinates.push_back(glm::vec3(initialVector.x + xDistance, initialVector.y + yDistance, 0));
			yDistance = yDistance + 0.05f;
			blockIndices.push_back(counter);
			counter++;
		}
		yDistance = yDistanceCopy;
		xDistance = xDistance + 0.05f;
	}
	//createVAO could later be repurposed to actually create the VAO
	//createVAO();

	loadTextures();
}


Block::~Block(){
	blockCoordinates.clear();
	blockIndices.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete blockShaderptr;
	blockShaderptr = nullptr;
	filepath1 = NULL;
	filepath2 = NULL;
}

void Block::createVAO(){

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockCoordinates[0])* blockCoordinates.size(), &blockCoordinates[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blockIndices[0])* blockIndices.size(), &blockIndices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// tex coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockCoordinates[0])* blockCoordinates.size(), &blockCoordinates[0], GL_STATIC_DRAW);

	//loadTextures();
	//shader_program = loadShaders("../Source/BLOCK_VERTEX_SHADER.vs", "../Source/BLOCK_FRAG_SHADER.frag");
}

vector<glm::vec3> Block::getBlockCoordinates(){
	return blockCoordinates;
}



void Block::draw(){
	/*glBindTexture(GL_TEXTURE_2D, boardTexture);
	glBindTexture(GL_TEXTURE_2D, sidewalkGrassTexture);
	glBindTexture(GL_TEXTURE_2D, boardTexture);*/
	Block::blockShaderptr->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	glUniform1i(glGetUniformLocation(blockShaderptr->Program, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sidewalkGrassTexture);
	glUniform1i(glGetUniformLocation(blockShaderptr->Program, "ourTexture2"), 1);
	

	glBindVertexArray(VAO);
	//glDrawElements(GL_POINTS, blockIndices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Block::printCoordinates(){
	for (GLuint i = 0; i < blockCoordinates.size(); i++){
		cout << blockCoordinates.at(i).x << " is x " << blockCoordinates.at(i).y << " is y " << endl;
	}
}

void Block::loadTextures(){
	blockShaderptr = (new Shader("../Source/BLOCK_VERTEX_SHADER.vs", "../Source/BLOCK_FRAG_SHADER.frag"));

	GLfloat vertices[] = {
		// Positions				// Colors				// Texture Coords
		-0.5f, 0.5f, 0.0f,			1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		// Road
		-0.5f, -0.5f, 0.0f,			1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 
		0.5f, 0.5f, 0.0f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,			0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
		
		
		-0.35f, 0.35f, 0.0f,		1.0f, 1.0f, 0.0f,		0.1375f, 0.8625f,	//Sidewalk profile
		-0.35f, -0.35f, 0.0f,		1.0f, 1.0f, 0.0f,		0.1375f, 0.1375f,
		0.35f, 0.35f, 0.0f,			1.0f, 1.0f, 0.0f,		0.8625f, 0.8625f,
		0.35f, -0.35f, 0.0f,		1.0f, 1.0f, 0.0f,		0.8625f, 0.1375f,

		-0.35f, 0.35f, 0.0125f,		1.0f, 0.0f, 0.0f,		0.15f, 0.85,	// Upper Square
		-0.35f, -0.35f, 0.0125f,	1.0f, 1.0f, 0.0f,		0.15f, 0.15f,
		0.35f, 0.35f, 0.0125f,		0.0f, 0.0f, 1.0f,		0.85f, 0.85f,
		0.35f, -0.35f, 0.0125f,		1.0f, 1.0f, 0.0f,		0.85f, 0.15f
	

	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2, // Road
		2,1,3,
		8,4,9,
		9,4,5,
		9,5,11,
		11,5,7,
		11,7,10,
		10,7,6,
		10,6,8,
		8,6,4,
		8,9,10,
		10,9,11
	};



	numInd = sizeof(indices);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	/// Texture 1
//	GLuint texture;
	glGenTextures(1, &boardTexture);
	glBindTexture(GL_TEXTURE_2D, boardTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(filepath1, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == '\0')
	{
		std::cout << "Unable to load image." << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	/// Texture 2
	glGenTextures(1, &sidewalkGrassTexture);
	glBindTexture(GL_TEXTURE_2D, sidewalkGrassTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	image = SOIL_load_image(filepath2, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == '\0')
	{
		std::cout << "Unable to load image." << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

}
