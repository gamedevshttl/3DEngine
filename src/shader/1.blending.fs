#version 330 core

in vec2 TexCoords;
out vec4 fragColor;

struct Material{
	sampler2D texture_diffuse;	
};
uniform Material material;


void main(){
	vec4 texColor = texture(material.texture_diffuse, TexCoords);	

	fragColor = texColor;
}
 

