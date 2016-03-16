#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoord;

//uniform mat4 projection;
//uniform mat4 view;

void main()
{
	gl_Position = vec4(position, 1.0f);
	//gl_Position = projection * view * vec4(position, 1.0f);
	
	TexCoord = position;
}