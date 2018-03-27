#include "object.h"

#include <glm/gtc/matrix_transform.hpp>

void object::set_position(const glm::vec3& position)
{
	m_model_matrix = glm::translate(m_model_matrix, position);
}

void object::set_rotation(float angle, const glm::vec3& vec)
{
	m_model_matrix = glm::rotate(m_model_matrix, glm::radians(angle), vec);
}

void object::set_scale(float value)
{
	m_model_matrix = glm::scale(m_model_matrix, glm::vec3(value));
}


void object::set_uniform_data(shader_logic& shader)
{
	shader.use();
	m_model_matrix_id = glGetUniformLocation(shader.get_id(), "model_matrix");
	m_mvp_matrix_id = glGetUniformLocation(shader.get_id(), "mvp_matrix");
}