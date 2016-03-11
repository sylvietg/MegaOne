#include "Block.h"


const char* Block::filepath = "../ThirdParty/Simple OpenGL Image Library/img_test.png";
vector<glm::vec3> Block::blockCoordinates;
vector<GLuint> Block::blockIndices;
Shader * Block::blockShaderptr = NULL;
GLuint Block::boardTexture;

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
	filepath = NULL;
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
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	Block::blockShaderptr->Use();

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

/*	GLfloat vertices[] = {
		// Positions          // Colors			  // Texture Coords
		0.5f, 0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};*/

	GLfloat vertices[] = {
		// Positions			// Colors				// Texture Coords
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Lower, Row 1
		-0.35f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.35f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.35f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Lower, Row 2
		-0.35f, 0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.35f, 0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.35f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Lower, Row 3
		-0.35f, -0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.35f, -0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Lower, Row 4
		-0.35f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.35f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.35f, 0.35f, 0.0125f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Upper Square
		-0.35f, -0.35f, 0.0125f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.35f, 0.35f, 0.0125f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.35f, -0.35f, 0.0125f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f

	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 4, 1, // Road
		1, 4, 5,
		1, 5, 2,
		2, 5, 6,
		2, 6, 3,
		3, 6, 7,
		4, 8, 5,
		5, 8, 9,
		6, 10, 7,
		7, 10, 11,
		8, 12, 9,
		9, 12, 13,
		9, 13, 10,
		10, 13, 14,
		10, 14, 11,
		11, 14, 15,
		16, 5, 17,	// Sidewalk-Profile
		17, 5, 6,
		19, 10, 17,
		17, 10, 6,
		18, 9, 19,
		19, 9, 10,
		16, 5, 18,
		18, 5, 9,
		16, 18, 17,	// Sidewalk+Grass
		17, 18, 19

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
	unsigned char* image = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}
