#include "texture_logic.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texture_logic::load_bmp_custom(std::string image_path)
{
	unsigned char header[54];
	unsigned int data_pos;
	unsigned int width, height;
	unsigned int image_size;
	unsigned char *data;

	FILE* file = fopen(image_path.c_str(), "rb");
	
	if (!file) {
		std::cout << "image counl not be opened" << std::endl;
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) {
		std::cout << "not correct bmp file" << std::endl;
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		std::cout << "not correct bmp file" << std::endl;
		return false;
	}

	data_pos = *(int*)&(header[0x0A]);
	image_size = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (image_size == 0) image_size = width * height * 3;
	if (data_pos == 0) data_pos = 54;

	data = new unsigned char[image_size];
	fread(data, 1, image_size, file);
	fclose(file);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return texture_id;
}

GLuint texture_logic::load_image(const std::string& image_path)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);	

	int width, height, nr_channels;
	unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nr_channels, 0);
	
	if (data) {

		GLenum format;
		if (nr_channels == 1)
			format = GL_RED;
		else if (nr_channels == 3)
			format = GL_RGB;
		else if (nr_channels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (nr_channels == 4) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed load texture " << image_path  << std::endl;
	}

	return texture_id;
}

GLuint texture_logic::load_cube_map(std::vector<std::string> faces)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	int width, height, nr_chaneels;

	for (unsigned int i = 0; i < faces.size(); ++i) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nr_chaneels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture faled to load at path" << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture_id;
}