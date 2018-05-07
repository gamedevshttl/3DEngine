#version 330 core

out vec4 frag_color;

in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;
in vec4 FragPosLightSpace;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

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

uniform sampler2D shadowMap;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);	

	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	 if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 CalcLightDir_old(DirLight light, vec3 normal, vec3 view_dir)
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

vec3 CalcLightDir(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 color = texture(material.texture_diffuse, tex_coord).rgb;
    normal = normalize(normal);
    //vec3 lightColor = vec3(0.3);
	vec3 lightColor = light.ambient;
    // ambient
    vec3 ambient = 1.0 * color;
    // diffuse
    //vec3 lightDir = normalize(lightPos - FragPos);

	vec3 lightDir = normalize(light.direction);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace, lightDir, normal);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	return lighting;
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