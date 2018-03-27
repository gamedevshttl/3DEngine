#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec2 tex_coord;
out vec3 frag_pos;

uniform mat4 model_matrix;
uniform mat4 mvp_matrix;

void main()
{
	tex_coord = aTexCoord;
	
	frag_pos = vec3(model_matrix * vec4(aPos, 1.0));

	normal = mat3(transpose(inverse(model_matrix))) * aNormal; 
	//normal = aNormal; 

	gl_Position = mvp_matrix * vec4(aPos, 1.0);
}