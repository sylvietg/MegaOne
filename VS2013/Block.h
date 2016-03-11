#pragma once
#include <vector>
#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include <iostream>
#include "Shader.h"
#include "SOIL.h"
#include "Utility.h"

using namespace std;
#define BLOCK_WIDTH 20
#define BLOCK_HEIGHT 20


class Block{
	public:
		Block();
		~Block();

		static Block* getBlock();
		vector<glm::vec3> getBlockCoordinates();
		void draw();
		void printCoordinates();
		void loadTextures();

	private:
		static const char* filepath;
		static GLuint boardTexture;
		//static GLuint sidewalkGrassTexture;
		static Shader *blockShaderptr;
		static vector<glm::vec3> blockCoordinates;
		static vector<GLuint> blockIndices;
		GLuint numInd;
		GLuint VAO, VBO, EBO;
		void createVAO();
		GLuint shader_program;
};
