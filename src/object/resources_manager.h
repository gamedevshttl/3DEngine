#ifndef __resources_manager__
#define __resources_manager__

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <map>

class resources_manager
{
public:
	static resources_manager& instance()
	{
		static resources_manager s_resources_manager;
		return s_resources_manager;
	}

	void add_texture(std::string name, GLuint id);
	const std::map<std::string, GLuint>& get_texture_map();

private:
	resources_manager() {}

	resources_manager(const resources_manager&) = delete;
	resources_manager& operator=(const resources_manager&) = delete;

private:
	std::map<std::string, GLuint> m_texture_map;
};

#endif

