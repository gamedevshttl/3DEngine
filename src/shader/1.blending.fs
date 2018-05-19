#version 330 core

in vec2 TexCoords;
out vec4 fragColor;

struct Material{
	sampler2D diffuse;	
};
uniform Material material;


void main(){
	vec4 texColor = texture(material.diffuse, TexCoords);	

	if(texColor.a < 0.1)
        discard;

	fragColor = texColor;
}
 

