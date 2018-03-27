#ifndef __texture_logic__
#define __texture_logic__

#include <GL/glew.h>
#include <string>

class texture_logic
{
public:
	GLuint load_bmp_custom(std::string image_path);
	GLuint load_image(const std::string& image_path);
};

#endif //__texture_logic__