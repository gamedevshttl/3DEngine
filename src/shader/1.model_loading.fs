#version 330 core

in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

struct Material{
	sampler2D texture_diffuse;	
	sampler2D texture_specular;
	float shinisess;
	sampler2D emission;
};
uniform Material material;


uniform vec3 viewPos;

out vec4 frag_color;

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
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 8.0);

//	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, tex_coord));
//	vec3 difuse = light.diffuse * diff * vec3(texture(texture_diffuse1, tex_coord));
//	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, tex_coord));

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, tex_coord));
	vec3 difuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, tex_coord));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, tex_coord));
	
	return ambient + difuse + specular;
}

float far = 100.0;
float near = 0.1;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{	
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(viewPos - frag_pos);

	vec3 result = CalcLightDir(dir_light, norm, view_dir);
	frag_color = vec4(result, 1.0);

	//float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	//frag_color = vec4(vec3(depth), 1.0);	
}