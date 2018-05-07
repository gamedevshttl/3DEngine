#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceMatrix;

out vec3 normal;
out vec2 tex_coord;
out vec3 frag_pos;
out vec4 FragPosLightSpace;

uniform mat4 model_matrix;
uniform mat4 mvp_matrix;
uniform mat4 lightSpaceMatrix;

void main()
{
	tex_coord = aTexCoord;	
	frag_pos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal; 

	FragPosLightSpace = lightSpaceMatrix * vec4(frag_pos, 1.0);

	gl_Position = mvp_matrix * aInstanceMatrix * vec4(aPos, 1.0f);
}