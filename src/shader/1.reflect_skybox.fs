#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;

struct Material{
	sampler2D texture_diffuse;
	samplerCube cube_texture_reflect;
};
uniform Material material;

void main(){
/*
	vec3 I = normalize(viewPos - Position);
	vec3 R = reflect(I, normalize(Normal));	
	FragColor = vec4(texture(material.cube_texture_reflect, R).rgb, 1.0);
*/

	float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(material.cube_texture_reflect, R).rgb, 1.0);

	//FragColor = vec4(1,0,0,1);
}