#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 mvp_matrix;

void main()
{
	gl_Position = mvp_matrix * vec4(aPos, 1);
}
