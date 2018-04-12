#include "model.h"
#include "../texture_logic.h"

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "resources_manager.h"

void model::init(const std::string& path, shader_logic& shader, const details_object& details_obj)
{
	load_model(path);
	m_model_matrix = glm::mat4(1.0f);

	set_uniform_data(shader);

	for (const auto& elem : details_obj.m_textures)
	{
		texture texture_item = elem;

		auto texture_it = std::find_if(m_textures.begin(), m_textures.end(), [texture_item](const texture& exists_texture) {return exists_texture.m_type == texture_item.m_type; });

		if (texture_it == m_textures.end()) {
			//if (elem.m_type == "cube_texture_reflect")
			//	load_cube_texture_reflect(texture_item);
			//else
				load_texture(texture_item);
		}
	}
}

void model::add_instance_matrix(GLuint amount, const glm::mat4& model)
{
	m_instance_amount = amount;

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &model[0], GL_STATIC_DRAW);

	for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		glBindVertexArray(it->get_VAO());

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
}

void model::add_instance_matrix_vector(const std::vector<glm::mat4>& matrix_vector)
{
	m_instance_matrix_vector.insert(m_instance_matrix_vector.end(), matrix_vector.begin(), matrix_vector.end());
}

void model::post_init()
{
	if (m_instance_matrix_vector.empty())
		return;

	m_instance_amount = m_instance_matrix_vector.size();

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_instance_matrix_vector.size() * sizeof(glm::mat4), &m_instance_matrix_vector[0][0], GL_STATIC_DRAW);

	for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		glBindVertexArray(it->get_VAO());

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
}

void model::load_texture(texture& texture_item)
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

void model::draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos)
{	
	shader.use();

	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	for (GLuint i = 0; i<m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		shader.set_int("material." + m_textures[i].m_type, i);

		//if (m_textures[i].m_type == "cube_texture_reflect")
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[i].m_id);
		//else
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	if (m_instance_amount == 0) {
		m_mvp_matrix = projection * view * m_model_matrix;
		glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);
		for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
			it->draw(shader);
	}
	else {
		m_mvp_matrix = projection * view;
		glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);
		for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
			it->draw_instance(m_instance_amount, shader);
		}
	}
}

void model::draw_instance(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos)
{
	shader.use();

	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);
	
	for (GLuint i = 0; i<m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		shader.set_int("material." + m_textures[i].m_type, i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	m_mvp_matrix = projection * view;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

	for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		it->draw_instance(m_instance_amount, shader);
	}
}

void model::draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model)
{
	shader.use();

	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &model[0][0]);

	m_mvp_matrix = projection * view * model;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

	for (GLuint i = 0; i<m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		shader.set_int("material." + m_textures[i].m_type, i);

		//if (m_textures[i].m_type == "cube_texture_reflect")
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[i].m_id);
		//else
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		it->draw(shader);
}

void model::set_uniform_data(shader_logic& shader)
{
	m_model_matrix_id = glGetUniformLocation(shader.get_id(), "model_matrix");	
	m_mvp_matrix_id = glGetUniformLocation(shader.get_id(), "mvp_matrix");
}

std::shared_ptr<object> model::clone()
{
	std::shared_ptr<object> clone_obj = std::make_shared<model>(*this);
	return clone_obj;
}

void model::load_model(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)	
	{
		std::cout << "error::assimp::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));
	process_node(scene->mRootNode, scene);
}

void model::process_node(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(process_mesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		process_node(node->mChildren[i], scene);
	}
}

mesh model::process_mesh(aiMesh *ai_mesh, const aiScene *scene)
{
	std::vector<vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<texture> textures;

	for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
		vertex vertex_item;
		
		glm::vec3 position;
		position.x = ai_mesh->mVertices[i].x;
		position.y = ai_mesh->mVertices[i].y;
		position.z = ai_mesh->mVertices[i].z;
		vertex_item.m_position = position;		

		if (ai_mesh->mNormals) {
			glm::vec3 normal;
			normal.x = ai_mesh->mNormals[i].x;
			normal.y = ai_mesh->mNormals[i].y;
			normal.z = ai_mesh->mNormals[i].z;
			vertex_item.m_normal = normal;
		}

		if (ai_mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = ai_mesh->mTextureCoords[0][i].x;
			vec.y = ai_mesh->mTextureCoords[0][i].y;

			vertex_item.m_tex_coords = vec;
		}
		else
			vertex_item.m_tex_coords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex_item);
	}

	for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
		aiFace face = ai_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	
	if (ai_mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];
		
		std::vector<texture> diffuse_map = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_map.begin(), diffuse_map.end());

		std::vector<texture> specular_map = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_map.begin(), specular_map.end());
	}
	
	return mesh(vertices, indices, textures);
}
	
std::vector<texture> model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name)
{
	std::vector<texture> textures;
	texture_logic txtr_logic;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);


		bool skip = false;
		for (std::vector<texture>::iterator it = m_textures_loaded.begin(); it != m_textures_loaded.end(); ++it) {
			if (strcmp(it->m_path.data(), str.C_Str()) == 0) {
				textures.push_back(*it);
				skip = true;
				break;
			}
		}


		texture texture_item;
		texture_item.m_id = txtr_logic.load_image(m_directory + "/" + str.C_Str());
		texture_item.m_type = type_name;
		texture_item.m_path = str.C_Str();

		textures.push_back(texture_item);
	}

	return textures;
}

