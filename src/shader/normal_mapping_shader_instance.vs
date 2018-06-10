#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstanceMatrix;
layout(location = 7) in vec3 aTangent;
layout(location = 8) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
//out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 TangentLightDirection;
out mat3 mat_TBN;

struct DirLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dir_light;

uniform mat4 mvp_matrix;
uniform mat4 model_matrix;
uniform mat4 lightSpaceMatrix;
uniform vec3 viewPos;

void main()
{
	FragPos = vec3(aInstanceMatrix * vec4(aPos, 1));
	Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	TexCoords = aTexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(aInstanceMatrix)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));  
	TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;

	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	

	TangentLightDirection = normalize(TBN * dir_light.direction);
	mat_TBN = TBN;
	
	gl_Position = mvp_matrix * aInstanceMatrix * vec4(aPos, 1.0f);
}