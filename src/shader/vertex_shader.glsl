#version 330 core

layout(location = 0) in vec3 vertexPostiion_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 vertex_normal;

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;

uniform mat4 MVP;
uniform mat4 model_matrix;

void main(){
	vec4 v = vec4(vertexPostiion_modelspace, 1);
	gl_Position =  MVP * v;
	
	tex_coords = texCoords;

	frag_pos = vec3(model_matrix * vec4(vertexPostiion_modelspace, 1.0f));

	normal = vertex_normal;
	normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;  
}