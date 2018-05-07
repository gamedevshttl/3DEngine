#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aInstanceMatrix;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
//	if (aInstanceMatrix)
		gl_Position = lightSpaceMatrix * model * aInstanceMatrix * vec4(aPos, 1.0);
//	else
//		gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);	
}