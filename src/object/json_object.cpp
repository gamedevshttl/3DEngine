#include "json_object.h"
#include "../texture_logic.h"

#include "rapidjson/document.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include "resources_manager.h"

#include <glm/gtc/matrix_transform.hpp>

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
	
	if (document.HasMember("quantity")) {
		m_quantity = document["quantity"].GetInt();
	}

	if (document.HasMember("rotation_angle")) {
		m_rotation_angle = document["rotation_angle"].GetFloat();
		m_rotation_angle = glm::radians(m_rotation_angle);
	}

	
	if (document.HasMember("step")) {
		const rapidjson::Value& vector_item = document["step"];
		if (vector_item.Size() == 3) {
			m_step.x = document["step"][0].GetFloat();
			m_step.y = document["step"][1].GetFloat();
			m_step.z = document["step"][2].GetFloat();
		}
	}

	if (m_quantity == 1 && details_obj.m_quantity > 1) {
		m_step = details_obj.m_step;
		m_quantity = details_obj.m_quantity;
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
	

	std::vector<normal_mapping_data> normal_mapping_data_vetor;

	for (const auto& elem : details_obj.m_textures)
	{
		texture texture_item = elem;

		auto texture_it = std::find_if(m_textures.begin(), m_textures.end(), [texture_item](const texture& exists_texture) {return exists_texture.m_type == texture_item.m_type; });

		if (texture_it == m_textures.end()) {
			if (elem.m_type == "cube_texture_reflect")
				load_cube_texture_reflect(texture_item);
			else if (elem.m_type == "texture_normal_mapping") {

				load_texture(texture_item);

				for (int i = 0; i < m_vertices.size(); i+=3) {
					glm::vec3 edge1 = m_vertices[i + 1].m_position - m_vertices[i].m_position;
					glm::vec3 edge2 = m_vertices[i + 2].m_position - m_vertices[i].m_position;
					glm::vec2 deltaUV1 = m_vertices[i + 1].m_tex_coords - m_vertices[i].m_tex_coords;
					glm::vec2 deltaUV2 = m_vertices[i + 2].m_tex_coords - m_vertices[i].m_tex_coords;

					GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

					glm::vec3 tangent1, bitangent1;

					tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
					tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
					tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
					tangent1 = glm::normalize(tangent1);

					bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
					bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
					bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
					bitangent1 = glm::normalize(bitangent1);

					normal_mapping_data normal_mapping_item;
					normal_mapping_item.m_tangent = tangent1;
					normal_mapping_item.m_bitangent = bitangent1;

					normal_mapping_data_vetor.push_back(normal_mapping_item);
					normal_mapping_data_vetor.push_back(normal_mapping_item);
					normal_mapping_data_vetor.push_back(normal_mapping_item);
				}
			}
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(vertex), &m_indices[0], GL_STATIC_DRAW);
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


	if (normal_mapping_data_vetor.size()) {
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, normal_mapping_data_vetor.size() * sizeof(normal_mapping_data), &normal_mapping_data_vetor[0], GL_STATIC_DRAW);

		glBindVertexArray(m_VAO);

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(normal_mapping_data), (void*)0);

		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(normal_mapping_data), (void*)offsetof(normal_mapping_data, m_bitangent));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	m_model_matrix = glm::mat4(1);
}

int json_object::get_quantity() const 
{
	return m_quantity;
}

float json_object::get_rotation_angle() const
{
	return m_rotation_angle;
}

const glm::vec3& json_object::get_step() const
{
	return m_step;
}

void json_object::set_in_space(const glm::vec3& position, float scale, float angle, const details_object& details_obj, std::vector<glm::mat4>& model_matrices_vector)
{
	int quantity = m_quantity;
	glm::vec3 step = m_step;

	if (details_obj.m_quantity) {
		quantity = details_obj.m_quantity;
		step = details_obj.m_step;
	}

	//std::vector<glm::mat4> model_matrices_vector;
	for (int i = 0; i < quantity; ++i) {
		glm::mat4 model = get_model_matrix();

		glm::vec3 local_step;
		local_step.x = step.x * i;
		local_step.y = step.y * i;
		local_step.z = step.z * i;

		model = glm::translate(model, position + local_step);
		model = glm::scale(model, glm::vec3(scale));

		float local_rotation = get_rotation_angle() * i;		

		model = glm::rotate(model, angle + local_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	
		model_matrices_vector.push_back(model);
	}

	//add_instance_matrix_vector(model_matrices_vector);
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

void json_object::draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, GLuint shadow_map)
{	
	shader.use();
	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	for(GLuint i = 0; i<m_textures.size(); ++i){

		glActiveTexture(GL_TEXTURE0 + i);								
		shader.set_int("material." + m_textures[i].m_type, i);
		
		if (m_textures[i].m_type == "cube_texture_reflect")
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[i].m_id);
		else if (m_textures[i].m_type == "texture_normal_mapping") {		
			glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
		}
		else
			glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);		
	}

	if (shadow_map) {
		glActiveTexture(GL_TEXTURE0 + m_textures.size());
		shader.set_int("shadowMap", m_textures.size());
		glBindTexture(GL_TEXTURE_2D, shadow_map);
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

		m_mvp_matrix = projection * view;
		glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

		if (!m_indices.empty())
			glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_instance_amount);
		else
			glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertices.size(), m_instance_amount);
	}

	glBindVertexArray(0);
}

void json_object::draw_shadow(shader_logic& shader)
{
	if (!m_shadow)
		return;

	shader.use();
	shader.set_mat4("model", m_model_matrix);

	glBindVertexArray(m_VAO);

	if (m_instance_amount == 0) {
		if (!m_indices.empty())
			glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	}
	else {
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