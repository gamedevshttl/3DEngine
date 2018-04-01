#ifndef __skybox__
#define __skybox__

#include "shader_logic.h"

class skybox
{
public:
	void init();
	void draw(const glm::mat4& projection, const glm::mat4& view);	
private:
	GLuint m_VAO;
	shader_logic m_shader;
	GLuint m_cubemap_texture_id;
};

#endif