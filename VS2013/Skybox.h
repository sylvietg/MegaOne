#pragma once

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <vector>
#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include <iostream>
#include "Shader.h"
#include "SOIL.h"

using namespace std;
class Skybox
{
	public:
		Skybox();
		~Skybox();
		void loadTexture();
		void createTexture();
		void createBuffers();
		void draw();
	private:
		static vector<glm::vec3> cubeV;
		static vector<GLuint> cubeInd;
		static vector<const char*> filepath;
		GLint faceTex;
		GLuint VAO, VBO, EBO;
		static Shader *skyShaderptr;
		GLuint shader_program;
		void loadIndices();
};

#endif /* SKYBOX_H_ */