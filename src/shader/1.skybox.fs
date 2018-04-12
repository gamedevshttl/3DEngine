#version 330 core
out vec4 FragColor;

in vec3 TexCoords;


struct Material{	
	samplerCube skybox;
};
uniform Material material;

//uniform samplerCube skybox;

void main(){
	FragColor = texture(material.skybox, TexCoords);
}