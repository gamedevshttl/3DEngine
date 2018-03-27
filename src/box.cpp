#include "box.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture_logic.h"

box::box()
{}

void box::init()
{
	glGenVertexArrays(1, &m_box_VAO);

	float g_vertex_buffer_data[] = {
		1.5f,  1.5f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
		1.5f, -1.5f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		-1.5f, -1.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		-1.5f,  1.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(m_box_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint program_id = m_box_shader.load_shader("../src/shader/box_vertex_shader.glsl", "../src/shader/box_fragment_shader.glsl");
	m_mvp_matrix_id = glGetUniformLocation(program_id, "mvp_matrix");
	m_model_matrix_id = glGetUniformLocation(program_id, "model_matrix");

	texture_logic txtr_logic;
	m_diffuse_texture = txtr_logic.load_image("../resources/texture/container2.png");
	m_specular_texture = txtr_logic.load_image("../resources/texture/container2_specular.png");

	m_box_shader.use();
	m_box_shader.set_int("material.diffuse", 0);
	m_box_shader.set_int("material.specular", 1);
	//m_box_shader.set_int("material.emission", 3);

	m_model_matrix = glm::mat4(1.0f);
	//m_mvp_matrix = projection * view * m_model_matrix;
}

void box::set_model_matrix(const glm::mat4& model_matrix)
{
	m_model_matrix = model_matrix;
}

void box::set_position(const glm::vec3& position)
{
	m_model_matrix = glm::translate(m_model_matrix, position);
}

void box::set_rotation(float angle, const glm::vec3& vec)
{
	m_model_matrix = glm::rotate(m_model_matrix, glm::radians(angle), vec);
}

void box::render(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos)
{	
	glBindVertexArray(m_box_VAO);

	m_box_shader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	//m_model_matrix = glm::mat4(1.0f);
	m_mvp_matrix = projection * view * m_model_matrix;

	//glm::vec3 view_pos = m_camera.get_pos();
	m_box_shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	m_mvp_matrix = projection * view * m_model_matrix;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}