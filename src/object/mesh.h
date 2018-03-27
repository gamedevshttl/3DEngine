#ifndef __mesh__
#define __mesh__

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../shader_logic.h"
#include "object.h"

class mesh
{
public:
	std::vector<vertex> m_verteces;
	std::vector<GLuint> m_indices;
	std::vector<texture> m_textures;

	mesh(const std::vector<vertex>& verteces, const std::vector<GLuint>& indices, const std::vector<texture>& textures);
	void draw(shader_logic& shader);

private:
	void setup_mesh();

	GLuint m_VAO, m_VBO, m_EBO;
};

#endif