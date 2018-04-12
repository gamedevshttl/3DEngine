#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 model_matrix;
uniform mat4 mvp_matrix;


void main()
{
	TexCoords = aTexCoords;
	gl_Position = mvp_matrix * aInstanceMatrix * vec4(aPos, 1.0);
}