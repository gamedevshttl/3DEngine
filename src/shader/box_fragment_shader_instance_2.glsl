#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

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

/*
vec3 CalcLightDir_old(DirLight light, vec3 normal, vec3 view_dir)
{
	float diff = max(dot(normal, light.direction), 0.0);
	
	vec3 reflect_dir = reflect(-light.direction, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
	
	//return ambient + diffuse + specular;

	float shadow = ShadowCalculation(FragPosLightSpace);        	        
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

	return lighting;
}
*/

vec3 CalcLightDir(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 color = texture(material.texture_diffuse, TexCoords).rgb;
    normal = normalize(normal);
    //vec3 lightColor = vec3(0.3);
	vec3 lightColor = light.ambient;
    // ambient
    vec3 ambient = 0.7 * color;
    // diffuse
    //vec3 lightDir = normalize(lightPos - FragPos);

	vec3 lightDir = normalize(light.direction);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
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



out vec4 frag_color;

void main()
{
	//frag_color = texture(material.diffuse, TexCoords);
	//frag_color = vec4(1.0, 0.0, 0.0, 1);

	vec3 norm = normalize(Normal);
	vec3 view_dir = normalize(viewPos - FragPos);

	vec3 result;
	result = CalcLightDir(dir_light, norm, view_dir);
	
	//for(int i = 0; i<4; ++i )
	//	result += CalcPoitLight(point_light_array[i], norm, FragPos, view_dir);

	//result += CalcPoitLight(point_light_array[3], norm, FragPos, view_dir);
	//result += CalcPoitLight(point_light_array[i], norm, FragPos, view_dir);

	frag_color = vec4(result, 1.0);
}
