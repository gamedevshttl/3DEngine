#include "json_utils.h"



glm::vec3 json::read_vector(const rapidjson::Value& value, const std::string key)
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