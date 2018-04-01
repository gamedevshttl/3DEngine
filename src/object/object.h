#ifndef __object__
#define __object__

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "../shader_logic.h"


struct vertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_tex_coords;
};

struct texture
{
	GLuint m_id;
	std::string m_type;
	std::string m_path;
};

class object: public std::enable_shared_from_this<object>
{
public:
	object()
		: is_exists_noraml(false)
	{}

	virtual void init(const std::string& path, shader_logic& shader) = 0;
	virtual void set_position(const glm::vec3& position);
	virtual void set_rotation(float angle, const glm::vec3& vec);
	virtual void set_scale(float value);

	virtual void draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos) = 0;

	virtual void draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model) {}

	virtual void set_uniform_data(shader_logic& shader);

	virtual ~object() {}

	virtual std::shared_ptr<object> clone() = 0;
	
	const glm::mat4& get_model_matrix() { return m_model_matrix; }

protected:
	GLuint m_model_matrix_id;
	glm::mat4 m_model_matrix;

	GLuint m_mvp_matrix_id;
	glm::mat4 m_mvp_matrix;

	bool is_exists_noraml;
};

#endif