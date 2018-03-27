#ifndef __shader_logic__
#define __shader_logic__

#include <GL/glew.h>
#include <string>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

class shader_logic
{
public:
	GLuint load_shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

	void use();
	void set_bool(const std::string& name, bool value);
	void set_int(const std::string& name, int value);
	void set_float(const std::string& name, float value);
	void set_vec3(const std::string& name, float a, float b, float c);

	void set_vec3(const std::string& name, const glm::vec3& vec);
	void set_mat4(const std::string &name, const glm::mat4 &mat);

	GLuint get_id() const;
private:
	std::string load_file(const std::string& path);
	void compile_shader(GLuint shader_id, const std::string& shader_code);

	GLuint program_id;
};

#endif //__shader_logic__