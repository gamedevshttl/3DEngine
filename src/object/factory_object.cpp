#include "factory_object.h"
#include "json_object.h"
#include "model.h"

#include <fstream>
#include <memory>

void factory_object::parce_scene(const std::string& scene_path)
{
	std::string file_content;
	std::ifstream stream(scene_path.c_str(), std::ios::in);
	if (stream) {
		std::string line;
		while (std::getline(stream, line)) {
			file_content += "\n" + line;
		}
	}
	
	rapidjson::Document document;
	document.Parse(file_content.c_str());

	assert(document.IsObject());

	std::vector<vertex> vertex_vector;

	const rapidjson::Value& object_array = document["object_array"];
	assert(object_array.IsArray());

	//m_vertices.reserve(vertex_buffer_data.Size());

	for (rapidjson::SizeType i = 0; i < object_array.Size(); ++i) {

		std::string path;
		if (object_array[i].HasMember("path")) {
			path = object_array[i]["path"].GetString();
		}

		std::string vertex_shader;
		if (object_array[i].HasMember("vertex_shader")) {
			vertex_shader = object_array[i]["vertex_shader"].GetString();
		}

		std::string fragment_shader;
		if (object_array[i].HasMember("fragment_shader")) {
			fragment_shader = object_array[i]["fragment_shader"].GetString();
		}
		
		glm::vec3 position = read_vector(object_array[i], "position");

		float scale = 1.0f;
		if (object_array[i].HasMember("scale")) {
			scale = object_array[i]["scale"].GetFloat();
		}

		int quantity = 0;
		if (object_array[i].HasMember("quantity")) {
			quantity = object_array[i]["quantity"].GetInt();
		}

		glm::vec3 step = read_vector(object_array[i], "step");

		std::shared_ptr<object> obj;
		std::shared_ptr<shader_logic> shader;

		auto it_shader = m_map_shader.find(vertex_shader + fragment_shader);
		if (it_shader == m_map_shader.end()) {

			shader = std::make_shared<shader_logic>();
			shader->load_shader(vertex_shader, fragment_shader);
			auto it_local_shader = m_map_shader.insert(std::make_pair(vertex_shader + fragment_shader, shader_map_object(shader)));

			if (it_local_shader.second)
				obj = create_object(path, *it_local_shader.first->second.m_shader.get());

			if (obj) {
				it_local_shader.first->second.m_map_object[path] = obj;				
			}
		}
		else {
			shader = it_shader->second.m_shader;

			auto it_obj = it_shader->second.m_map_object.find(path);
			if (it_obj == it_shader->second.m_map_object.end()) {				
				obj = create_object(path, *it_shader->second.m_shader.get());
				if (obj)
					it_shader->second.m_map_object[path] = obj;
			}
			else
				obj = it_obj->second;
		}

		if (obj) {			
			if(m_map_vector_object.find(shader->get_id()) == m_map_vector_object.end())
				m_map_vector_object[shader->get_id()].m_shader = shader;

			if (quantity > 0) {
				for (int i = 0; i < quantity; ++i) {
					std::shared_ptr<object> obj_clone = obj->clone();
					if (obj_clone) {
						glm::vec3 local_step;
						local_step.x = step.x * i;
						local_step.y = step.y * i;
						local_step.z = step.z * i;
						obj_clone->set_position(position + local_step);
						obj_clone->set_scale(scale);
						m_map_vector_object[shader->get_id()].m_vector_object.push_back(obj_clone);
					}
				}
			}
			else {
				std::shared_ptr<object> obj_clone = obj->clone();
				if (obj_clone) {
					obj_clone->set_position(position);
					obj_clone->set_scale(scale);
					m_map_vector_object[shader->get_id()].m_vector_object.push_back(obj_clone);
				}
			}
		}
	}
}

glm::vec3 factory_object::read_vector(const rapidjson::Value& value, const std::string key)
{
	glm::vec3 vector;
	if (value.HasMember(key.c_str())) {
		const rapidjson::Value& vector_item = value[key.c_str()];
		if (vector_item.Size() == 3) {
			vector.x = value[key.c_str()][0].GetFloat();
			vector.y = value[key.c_str()][1].GetFloat();
			vector.z = value[key.c_str()][2].GetFloat();
		}
	}

	return vector;
}

std::shared_ptr<object> factory_object::create_object(const std::string& object_name, shader_logic& shader)
{
	if (object_name.find(".json") != std::string::npos) {
		std::shared_ptr<object> obj = std::make_shared<json_object>();
		//obj->init("../resources/objects_data/plane_data.json", shader);
		obj->init(object_name, shader);
		return obj;
	}
	else if(object_name.find(".obj") != std::string::npos)
	{
		std::shared_ptr<object> obj = std::make_shared<model>();
		obj->init(object_name, shader);
		return obj;
	}

	return std::shared_ptr<object>();
}

std::map<int, shader_vector_object>& factory_object::get_map_vector_object()
{
	return m_map_vector_object;
}