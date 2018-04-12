#include "skybox.h"
#include "texture_logic.h"
#include "object\resources_manager.h"

#include <vector>
#include <string>


void skybox::init()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint VBO;
	glGenVertexArrays(1, &m_VAO);		
	glGenBuffers(1, &VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	auto it_texture = resources_manager::instance().get_texture_map().find("../resources/texture/skybox_1/");
	if (it_texture != resources_manager::instance().get_texture_map().end()) {
		m_cubemap_texture_id = it_texture->second;
	}
	else {
		std::vector<std::string> faces = {
			"../resources/texture/skybox_1/right.jpg",
			"../resources/texture/skybox_1/left.jpg",
			"../resources/texture/skybox_1/top.jpg",
			"../resources/texture/skybox_1/bottom.jpg",
			"../resources/texture/skybox_1/front.jpg",
			"../resources/texture/skybox_1/back.jpg",
		};

		m_cubemap_texture_id = texture_logic::instance().load_cube_map(faces);

		resources_manager::instance().add_texture("../resources/texture/skybox_1/", m_cubemap_texture_id);
	}

	m_shader.load_shader("../src/shader/1.skybox.vs", "../src/shader/1.skybox.fs");
	m_shader.use();
	//m_shader.set_int("skybox", 0);
	m_shader.set_int("material.skybox", 0);
	
}

void skybox::draw(const glm::mat4& projection, const glm::mat4& view)
{
	glDepthFunc(GL_LEQUAL);
	m_shader.use();
	glm::mat4 skybox_view = glm::mat4(glm::mat3(view));
	m_shader.set_mat4("view", skybox_view);
	m_shader.set_mat4("projection", projection);

	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap_texture_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}