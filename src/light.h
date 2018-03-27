#ifndef __light__
#define __light__

#include "shader_logic.h"
#include "rapidjson/document.h"

#include <vector>

struct point_light{
	glm::vec3 m_position;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_constant;
	float m_linear;
	float m_quadratic;
};

struct direction_light{
	glm::vec3 m_direction;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

class light
{
public:
	glm::vec3 read_vector(const rapidjson::Value& value, const std::string key);
	void create_light(const std::string& path);
	void set_uniform_light(shader_logic& shader);

	const std::vector<point_light>& get_point_light_vetor() { return m_point_light_vetor; }
	const direction_light& get_direction_light(){ return m_direction_light; }

private:
	std::vector<point_light> m_point_light_vetor;
	direction_light m_direction_light;
};

#endif