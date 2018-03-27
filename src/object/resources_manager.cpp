#include "resources_manager.h"

void resources_manager::add_texture(std::string name, GLuint id)
{
	m_texture_map[name] = id;
}

const std::map<std::string, GLuint>& resources_manager::get_texture_map()
{
	return m_texture_map;
}