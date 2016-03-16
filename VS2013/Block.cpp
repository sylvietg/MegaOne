#include "Block.h"


//const char* Block::filepath = "../ThirdParty/Simple OpenGL Image Library/test_block.png";
const char* Block::filepath1 = "../Images/road.png";
const char* Block::filepath2 = "../Images/sidewalk.png";
vector<glm::vec3> Block::blockCoordinates;
vector<GLuint> Block::blockIndices;
Shader * Block::blockShaderptr = NULL;
GLuint Block::boardTexture;
GLuint Block::sidewalkGrassTexture;

//default constructor possibly obsolete now
Block::Block(){

	/*	NOTE
	*	The grid isn't used and isn't useful right now
	*	It could come in handy when we use those bumpmaps(?)
	*/
	/*
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
	*/
	//loadTextures();
}

Block::Block(GLuint x, GLuint y){

	/*	NOTE
	*	The grid isn't used and isn't useful right now
	*	It could come in handy when we use those bumpmaps(?)
	*/
	/*
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
	*/
	loadTextures(x,y);
}



Block::~Block(){
	blockCoordinates.clear();
	blockIndices.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	delete blockShaderptr;
	blockShaderptr = nullptr;
	filepath1 = NULL;
	filepath2 = NULL;
}

void Block::createVAO(){

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockCoordinates[0])* blockCoordinates.size(), &blockCoordinates[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blockIndices[0])* blockIndices.size(), &blockIndices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockCoordinates[0])* blockCoordinates.size(), &blockCoordinates[0], GL_STATIC_DRAW);
}

vector<glm::vec3> Block::getBlockCoordinates(){
	return blockCoordinates;
}

void Block::draw(){
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	
	Block::blockShaderptr->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	glUniform1i(glGetUniformLocation(blockShaderptr->Program, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sidewalkGrassTexture);
	glUniform1i(glGetUniformLocation(blockShaderptr->Program, "ourTexture2"), 1);
	
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Block::printCoordinates(){
	for (GLuint i = 0; i < blockCoordinates.size(); i++){
		cout << blockCoordinates.at(i).x << " is x " << blockCoordinates.at(i).y << " is y " << endl;
	}
}

void Block::loadTextures(GLuint x, GLuint y){
	blockShaderptr = (new Shader("../Source/BLOCK_VERTEX_SHADER.vs", "../Source/BLOCK_FRAG_SHADER.frag"));
	GLfloat xoffset = x*1.f;
	GLfloat yoffset = y*1.f;


	GLfloat road[] = {
		// Positions									// Colors				// Texture Coords
		-0.5f + xoffset, 0.5f + yoffset,0.0f,			1.0f, 0.0f, 1.0f,		0.0f, 1.0f,			// Road
		-0.5f + xoffset, -0.5f + yoffset,0.0f,			1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		0.5f + xoffset, 0.5f + yoffset,	0.0f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		0.5f + xoffset, -0.5f + yoffset,0.0f,			0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	};

	GLfloat sidewalk[] = {
		// Positions									// Colors				// Texture Coords
		-0.35f + xoffset, 0.35f + yoffset, 0.0f,		1.0f, 1.0f, 0.0f,		0.1375f, 0.8625f,	//Sidewalk profile
		-0.35f + xoffset, -0.35f + yoffset, 0.0f,		1.0f, 1.0f, 0.0f,		0.1375f, 0.1375f,
		0.35f + xoffset, 0.35f + yoffset, 0.0f,			1.0f, 1.0f, 0.0f,		0.8625f, 0.8625f,
		0.35f + xoffset, -0.35f + yoffset, 0.0f,		1.0f, 1.0f, 0.0f,		0.8625f, 0.1375f,

		-0.35f + xoffset, 0.35f + yoffset, 0.0125f,		1.0f, 0.0f, 0.0f,		0.15f, 0.85f,		// Upper Square
		-0.35f + xoffset, -0.35f + yoffset, 0.0125f,	1.0f, 1.0f, 0.0f,		0.15f, 0.15f,
		0.35f + xoffset, 0.35f + yoffset, 0.0125f,		0.0f, 0.0f, 1.0f,		0.85f, 0.85f,
		0.35f + xoffset, -0.35f + yoffset, 0.0125f,		1.0f, 1.0f, 0.0f,		0.85f, 0.15f
	};

	GLuint roadInd[] = {  // Note that we start from 0!
		0, 1, 2, 
		2, 1, 3
	};

	GLuint sidewalkInd[] = {  // Note that we start from 0!
		4,0,5,
		5,0,1,
		5,1,7,
		7,1,3,
		7,3,6,
		6,3,2,
		6,2,4,
		4,2,0
	};

	roadIndSz = sizeof(roadInd);
	sidewalkIndSz = sizeof(sidewalkInd);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	glBindVertexArray(VAO);

	/// Road
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(road), road, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, roadIndSz, roadInd, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	/// Sidewalk
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sidewalk), sidewalk, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sidewalkIndSz, sidewalkInd, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(3);
	// Color attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	// TexCoord attribute
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(5);

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
