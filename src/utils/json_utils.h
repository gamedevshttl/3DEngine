#include "rapidjson/document.h"
#include <glm/glm.hpp>

class json
{
public:
	static json& instance()
	{
		static json s_json;
		return s_json;
	}

	glm::vec3 read_vector(const rapidjson::Value& value, const std::string key);
};