#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model_matrix;
uniform mat4 mvp_matrix;

void main(){
	Normal = mat3(transpose(inverse(model_matrix))) * aNormal;
	Position = vec3(model_matrix * vec4(aPos, 1.0));
	gl_Position = mvp_matrix * vec4(aPos, 1.0);
}