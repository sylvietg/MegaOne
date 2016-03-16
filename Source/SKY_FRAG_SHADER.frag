#version 330 core
in vec3 TexCoord;
out vec4 color;

// Texture samplers
uniform samplerCube cubemap;

void main()
{
	color = texture(cubemap, TexCoord);
}

