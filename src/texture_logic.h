#ifndef __texture_logic__
#define __texture_logic__

#include <GL/glew.h>
#include <string>
#include <vector>

class texture_logic
{
public:
	static texture_logic& instance()
	{
		static texture_logic s_texture_logic;
		return s_texture_logic;
	}

	GLuint load_bmp_custom(std::string image_path);
	GLuint load_image(const std::string& image_path);
	GLuint load_cube_map(std::vector<std::string> faces);

};

#endif //__texture_logic__