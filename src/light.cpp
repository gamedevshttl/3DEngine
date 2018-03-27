#include "light.h"

#include <fstream>
#include <iostream>
#include <vector>

//#include "rapidjson/document.h"

void light::create_light(const std::string& path)
{
	std::string file_content;
	std::ifstream stream(path.c_str(), std::ios::in);
	if (stream) {
		std::string line;
		while (std::getline(stream, line)) {
			file_content += "\n" + line;
		}
	}

	rapidjson::Document document;
	document.Parse(file_content.c_str());

	assert(document.IsObject());

	std::vector<point_light> point_light_vector;

	const rapidjson::Value& point_light_array = document["point_light_array"];
	assert(point_light_array.IsArray());

	point_light_vector.reserve(point_light_array.Size());

	for (rapidjson::SizeType i = 0; i < point_light_array.Size(); ++i) {

		point_light point_light_item;

		point_light_item.m_position = read_vector(point_light_array[i], "position");
		point_light_item.m_ambient = read_vector(point_light_array[i], "ambient");
		point_light_item.m_diffuse = read_vector(point_light_array[i], "diffuse");
		point_light_item.m_specular = read_vector(point_light_array[i], "specular");
		
		if (point_light_array[i].HasMember("constant"))
			point_light_item.m_constant = point_light_array[i]["constant"].GetFloat();

		if (point_light_array[i].HasMember("linear"))
			point_light_item.m_linear = point_light_array[i]["linear"].GetFloat();

		if (point_light_array[i].HasMember("quadratic"))
			point_light_item.m_quadratic = point_light_array[i]["quadratic"].GetFloat();

		
		m_point_light_vetor.push_back(point_light_item);
	}

	const rapidjson::Value& direction_light = document["direction_light"];

	if (direction_light.IsObject()) {
		m_direction_light.m_direction = read_vector(direction_light, "direction");
		m_direction_light.m_ambient = read_vector(direction_light, "ambient");
		m_direction_light.m_diffuse = read_vector(direction_light, "diffuse");
		m_direction_light.m_specular = read_vector(direction_light, "specular");
	}	
}

glm::vec3 light::read_vector(const rapidjson::Value& value, const std::string key)
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

void light::set_uniform_light(shader_logic& shader)
{
	shader.use();

	int i = 0;
	for (auto elem : m_point_light_vetor) {
		std::string number = std::to_string(i++);

		shader.set_vec3("point_light_array[" + number + "].position", elem.m_position);
		shader.set_vec3("point_light_array[" + number + "].ambient", elem.m_ambient);
		shader.set_vec3("point_light_array[" + number + "].diffuse", elem.m_diffuse);
		shader.set_vec3("point_light_array[" + number + "].specular", elem.m_specular);
		shader.set_float("point_light_array[" + number + "].constant", elem.m_constant);
		shader.set_float("point_light_array[" + number + "].linear", elem.m_linear);
		shader.set_float("point_light_array[" + number + "].quadratic", elem.m_quadratic);
	}

	shader.set_vec3("dir_light.direction", m_direction_light.m_direction);
	shader.set_vec3("dir_light.ambient", m_direction_light.m_ambient);
	shader.set_vec3("dir_light.diffuse", m_direction_light.m_diffuse);
	shader.set_vec3("dir_light.specular", m_direction_light.m_specular);
}