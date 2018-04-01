#ifndef __post_processing__
#define __post_processing__

#include "shader_logic.h"

class post_processing
{
public:
	void init();
	void pre_draw();
	void draw();
private:
	GLuint m_framebuffer;
	GLuint m_quadVAO;
	GLuint m_texture_color_buffer;
	shader_logic m_framebuffers_shader;	
};

#endif