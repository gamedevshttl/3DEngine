#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material{
	sampler2D texture_diffuse;	
	sampler2D texture_specular;
	float shinisess;
	sampler2D emission;
};
uniform Material material;

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
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
	
	return ambient + diffuse + specular;
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
uniform PointLight point_light_array[4];

vec3 CalcPoitLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - frag_pos);
	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 specular = light.diffuse * spec * vec3(texture(material.texture_specular, TexCoords));

	return ambient + diffuse + specular;
}

uniform vec3 viewPos;

out vec4 frag_color;

void main()
{
	//frag_color = texture(material.diffuse, TexCoords);
	//frag_color = vec4(1.0, 0.0, 0.0, 1);

	vec3 norm = normalize(Normal);
	vec3 view_dir = normalize(viewPos - FragPos);

	vec3 result;
	result = CalcLightDir(dir_light, norm, view_dir);
	
	for(int i = 0; i<4; ++i )
		result += CalcPoitLight(point_light_array[i], norm, FragPos, view_dir);

	//result += CalcPoitLight(point_light_array[3], norm, FragPos, view_dir);
	//result += CalcPoitLight(point_light_array[i], norm, FragPos, view_dir);

	frag_color = vec4(result, 1.0);
}
