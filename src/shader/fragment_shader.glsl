#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D emission;
};
uniform Material material;

in vec2 tex_coords;

in vec3 frag_pos;
in vec3 normal;

out vec4 frag_color;

uniform vec4 uniform_color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;


struct DirLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dir_light;

vec3 CalcLightDir(DirLight light, vec3 normal, vec3 view_dir)
{
	float diff = max(dot(normal, light.direction), 0.0);

	vec3 reflect_dir = reflect(-light.direction, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

	return (ambient + diffuse + specular);
}


struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHT 5
uniform PointLight point_light_array[NR_POINT_LIGHT];

vec3 CalPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - frag_pos);
	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);	
}


void main(){
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(viewPos - frag_pos);

	vec3 result;
	result = CalcLightDir(dir_light, norm, view_dir);

	for(int i = 3; i < NR_POINT_LIGHT; ++i)
		result += CalPointLight(point_light_array[i], norm, frag_pos, view_dir);

	result += CalPointLight(point_light_array[4], norm, frag_pos, view_dir);

	frag_color = vec4(result, 1.0);
}