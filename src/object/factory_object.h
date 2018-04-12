#ifndef __factory_object__
#define __factory_object__

#include "object.h"
#include <memory>
#include <vector>
#include <map>

#include "rapidjson/document.h"

struct shader_vector_object {
	std::shared_ptr<shader_logic> m_shader;
	std::vector<std::shared_ptr<object>> m_vector_object;
};

class factory_object
{
public:

	void parce_scene(const std::string& scene_path);
	glm::vec3 read_vector(const rapidjson::Value& value, const std::string key);
//protected:
	std::shared_ptr<object> create_object(const std::string& object_name, shader_logic& shader, const details_object& details_obj = details_object());

	std::map<int, shader_vector_object>& get_map_vector_object();

protected:
	std::vector<std::shared_ptr<object>> m_vector_object;

	struct shader_map_object{
		shader_map_object(const std::shared_ptr<shader_logic>& shader):m_shader(shader)
		{}

		std::shared_ptr<shader_logic> m_shader;
		std::map<std::string, std::shared_ptr<object>> m_map_object;
	};

	std::map<std::string, shader_map_object> m_map_shader;

	std::map<int, shader_vector_object> m_map_vector_object;

	std::map<std::string, std::shared_ptr<object>> m_map_instance_object;

};

#endif