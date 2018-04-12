#include "json_object.h"
#include "../texture_logic.h"

#include "rapidjson/document.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include "resources_manager.h"

json_object::~json_object()
{
	//glDeleteVertexArrays(1, &m_VAO);
}

void json_object::init(const std::string& path, shader_logic& shader, const details_object& details_obj)
{	
	set_uniform_data(shader);

	std::string file_content = load_file(path);

	rapidjson::Document document;
	document.Parse(file_content.c_str());

	assert(document.IsObject());
	
	std::vector<vertex> vertex_vector;

	const rapidjson::Value& vertex_buffer_data = document["vertex_buffer_data"];
	assert(vertex_buffer_data.IsArray());

	m_vertices.reserve(vertex_buffer_data.Size());

	for (rapidjson::SizeType i = 0; i < vertex_buffer_data.Size(); ++i) {
		
		vertex vertex_item;
		
		if (vertex_buffer_data[i].HasMember("p")) {
			const rapidjson::Value& vertex_item_p = vertex_buffer_data[i]["p"];
			if (vertex_item_p.Size() == 3){
				vertex_item.m_position.x = vertex_buffer_data[i]["p"][0].GetFloat();
				vertex_item.m_position.y = vertex_buffer_data[i]["p"][1].GetFloat();
				vertex_item.m_position.z = vertex_buffer_data[i]["p"][2].GetFloat();
			}
		}

		if (vertex_buffer_data[i].HasMember("tx")) {
			const rapidjson::Value& vertex_item_tx = vertex_buffer_data[i]["tx"];
			if (vertex_item_tx.Size() == 2) {
				vertex_item.m_tex_coords.x = vertex_buffer_data[i]["tx"][0].GetFloat();
				vertex_item.m_tex_coords.y = vertex_buffer_data[i]["tx"][1].GetFloat();
			}
		}

		if (vertex_buffer_data[i].HasMember("n")) {
			is_exists_noraml = true;
			const rapidjson::Value& vertex_item_n = vertex_buffer_data[i]["n"];
			if (vertex_item_n.Size() == 3) {
				vertex_item.m_normal.x = vertex_item_n[0].GetFloat();
				vertex_item.m_normal.y = vertex_item_n[1].GetFloat();
				vertex_item.m_normal.z = vertex_item_n[2].GetFloat();
			}
		}

		m_vertices.push_back(vertex_item);
	}

	if (document.HasMember("indices")) {

		const rapidjson::Value& indices = document["indices"];

		m_indices.reserve(indices.Size());
		for (rapidjson::SizeType i = 0; i < indices.Size(); ++i) {
			m_indices.push_back(indices[i].GetFloat());
		}
	}

	std::vector<std::string> texture_type_vector{ "texture_diffuse" , "texture_specular"};

	for (const auto& elem : texture_type_vector)
	{
		if (document.HasMember(elem.c_str())) {
			texture texture_item;
			texture_item.m_path = document[elem.c_str()].GetString();
			texture_item.m_type = elem;

			if (texture_item.m_type == "cube_texture_reflect")
				load_cube_texture_reflect(texture_item);
			else
				load_texture(texture_item);			
		}
	}
	
	for (const auto& elem : details_obj.m_textures)
	{
		texture texture_item = elem;

		auto texture_it = std::find_if(m_textures.begin(), m_textures.end(), [texture_item](const texture& exists_texture) {return exists_texture.m_type == texture_item.m_type; });

		if (texture_it == m_textures.end()) {
			if (elem.m_type == "cube_texture_reflect")
				load_cube_texture_reflect(texture_item);
			else
				load_texture(texture_item);
		}
	}

	glGenVertexArrays(1, &m_VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), &m_vertices[0], GL_STATIC_DRAW);

	if (!m_indices.empty()) {
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

	if (is_exists_noraml) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, m_normal));
	}

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, m_tex_coords));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_model_matrix = glm::mat4(1);
}

void json_object::add_instance_matrix(GLuint amount, const glm::mat4& model)
{
	m_instance_amount = amount;

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &model[0], GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	GLsizei vec4_size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(vec4_size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
	
}

void json_object::add_instance_matrix_vector(const std::vector<glm::mat4>& matrix_vector)
{	
	m_instance_matrix_vector.insert(m_instance_matrix_vector.end(), matrix_vector.begin(), matrix_vector.end());
}

void json_object::post_init()
{
	if (m_instance_matrix_vector.empty())
		return;

	m_instance_amount = m_instance_matrix_vector.size();

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_instance_matrix_vector.size() * sizeof(glm::mat4), &m_instance_matrix_vector[0][0], GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	GLsizei vec4_size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(vec4_size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void json_object::load_texture(texture& texture_item)
{
	texture_logic txtr_logic;

	if (!texture_item.m_path.empty()) {
		auto it_texture = resources_manager::instance().get_texture_map().find(texture_item.m_path);
		if (it_texture != resources_manager::instance().get_texture_map().end()) {
			texture_item.m_id = it_texture->second;
			m_textures.push_back(texture_item);
		}
		else {
			texture_item.m_id = txtr_logic.load_image(texture_item.m_path);
			m_textures.push_back(texture_item);

			resources_manager::instance().add_texture(texture_item.m_path, texture_item.m_id);
		}
	}
}

void json_object::load_cube_texture_reflect(texture& texture_item)
{
	auto it_texture = resources_manager::instance().get_texture_map().find(texture_item.m_path);
	if (it_texture != resources_manager::instance().get_texture_map().end()) {
		texture_item.m_id = it_texture->second;
		m_textures.push_back(texture_item);
	}
	else {
		std::vector<std::string> faces = {
			texture_item.m_path + "right.jpg",
			texture_item.m_path + "left.jpg",
			texture_item.m_path + "top.jpg",
			texture_item.m_path + "bottom.jpg",
			texture_item.m_path + "front.jpg",
			texture_item.m_path + "back.jpg",
		};

		texture_item.m_id = texture_logic::instance().load_cube_map(faces);
		m_textures.push_back(texture_item);
		resources_manager::instance().add_texture(texture_item.m_path, texture_item.m_id);
	}
}

void json_object::draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos)
{	
	shader.use();
	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	for(GLuint i = 0; i<m_textures.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + i);								
		shader.set_int("material." + m_textures[i].m_type, i);
		
		if (m_textures[i].m_type == "cube_texture_reflect")
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[i].m_id);
		else
			glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);		
	}

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);
	
	glBindVertexArray(m_VAO);

	if (m_instance_amount == 0) {
		m_mvp_matrix = projection * view * m_model_matrix;
		glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

		if (!m_indices.empty())
			glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	}
	else {

		if (m_instance_amount == 20)
			return;

		m_mvp_matrix = projection * view;
		glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

		if (!m_indices.empty())
			glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_instance_amount);
		else
			glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertices.size(), m_instance_amount);
	}

	glBindVertexArray(0);
}

void json_object::draw_instance(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos)
{
	shader.use();
	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	m_mvp_matrix = projection * view;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

	for (GLuint i = 0; i<m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		shader.set_int("material." + m_textures[i].m_type, i);

		if (m_textures[i].m_type == "cube_texture_reflect")
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[i].m_id);
		else
			glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	glBindVertexArray(m_VAO);
	if (!m_indices.empty())		
		glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_instance_amount);
	else		
		glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertices.size(), m_instance_amount);

	glBindVertexArray(0);
}

void json_object::draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model)
{
	shader.use();
	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &model[0][0]);

	m_mvp_matrix = projection * view * model;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

	for (GLuint i = 0; i<m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		//shader.set_int(m_textures[i].m_type, 0);
		shader.set_int("material.diffuse", 0);


		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	glBindVertexArray(m_VAO);
	if (!m_indices.empty())
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);
}

std::string json_object::load_file(const std::string& path)
{
	std::string file_content;
	std::ifstream stream(path.c_str(), std::ios::in);
	if (stream) {
		std::string line;
		while (std::getline(stream, line)) {
			file_content += "\n" + line;
		}
	}

	return file_content;
}

std::shared_ptr<object> json_object::clone()
{
	std::shared_ptr<object> clone_obj = std::make_shared<json_object>(*this);
	return clone_obj;
}