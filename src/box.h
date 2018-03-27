#ifndef __box__
#define __box__

#include "shader_logic.h"

#include <vector>

class box
{
public:
	box();

	void init();
	void render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos);

	void set_model_matrix(const glm::mat4& model_matrix);
	void set_position(const glm::vec3& position);
	void set_rotation(float angle, const glm::vec3& vec);

private:
	GLuint m_box_VAO;
	shader_logic m_box_shader;

	GLuint m_mvp_matrix_id;
	GLuint m_model_matrix_id;

	GLuint m_diffuse_texture;
	GLuint m_specular_texture;

	glm::mat4 m_model_matrix;
	glm::mat4 m_mvp_matrix;
};

#endif