#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstanceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

void main()
{
	/*
	FragPos = vec3(model_matrix * vec4(aPos, 1));
	Normal = mat3(transpose(inverse(model_matrix))) * aNormal;
	TexCoords = aTexCoord;
	
	gl_Position = mvp_matrix * vec4(aPos, 1);
	*/

	FragPos = vec3(aInstanceMatrix * vec4(aPos, 1));
	Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	TexCoords = aTexCoord;
	
	gl_Position = mvp_matrix * aInstanceMatrix * vec4(aPos, 1.0f);
}