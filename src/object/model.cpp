#include "model.h"
#include "../texture_logic.h"

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

void model::init(const std::string& path, shader_logic& shader, const details_object& details_obj)
{
	load_model(path);
	m_model_matrix = glm::mat4(1.0f);

	set_uniform_data(shader);
}

//void model::set_position(const glm::vec3& position)
//{
//	m_model_matrix = glm::translate(m_model_matrix, position);
//}
//
//void model::set_rotation(float angle, const glm::vec3& vec)
//{
//	m_model_matrix = glm::rotate(m_model_matrix, glm::radians(angle), vec);
//}

void model::draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos)
{	
	shader.use();

	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	m_mvp_matrix = projection * view * m_model_matrix;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

	shader.set_vec3("viewPos", view_pos.x, view_pos.y, view_pos.z);

	for (std::vector<mesh>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		it->draw(shader);
}

void model::draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view)
{
	shader.use();

	glUniformMatrix4fv(m_model_matrix_id, 1, GL_FALSE, &m_model_matrix[0][0]);

	m_mvp_matrix = projection * view * m_model_matrix;
	glUniformMatrix4fv(m_mvp_matrix_id, 1, GL_FALSE, &m_mvp_matrix[0][0]);

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
		texture_item.m_id = texture_logic::instance().load_image(m_directory + "/" + str.C_Str());
		texture_item.m_type = type_name;
		texture_item.m_path = str.C_Str();

		textures.push_back(texture_item);
	}

	return textures;
}

