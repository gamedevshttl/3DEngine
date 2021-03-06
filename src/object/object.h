#ifndef __object__
#define __object__

#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../shader_logic.h"


struct vertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_tex_coords;
};

struct normal_mapping_data
{
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent;	
};

struct vertex_normal_vector
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_tex_coords;
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent;
};



struct texture
{
	GLuint m_id;
	std::string m_type;
	std::string m_path;
};

struct details_object
{
	details_object() 
		: m_step(glm::vec3(0.0f))
		, m_quantity(0)
	{}

	std::vector<texture> m_textures;
	glm::vec3 m_step;
	int m_quantity;
};

class object: public std::enable_shared_from_this<object>
{
public:
	object()
		: is_exists_noraml(false)
	{}

	virtual void init(const std::string& path, shader_logic& shader, const details_object& details_obj = details_object()) = 0;
	virtual void add_instance_matrix_vector(const std::vector<glm::mat4>& matrix_vector) {}
	virtual void add_instance_matrix(GLuint amount, const glm::mat4& model) {}
	virtual void set_position(const glm::vec3& position);
	virtual void set_rotation(float angle, const glm::vec3& vec);
	virtual void set_scale(float value);
	virtual void post_init() {};

	virtual void draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, GLuint shadow_map = 0) = 0;

	virtual void draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model) {}

	virtual void draw_shadow(shader_logic& shader) {};

	virtual void set_uniform_data(shader_logic& shader);

	virtual ~object() {}

	virtual std::shared_ptr<object> clone() = 0;
	
	const glm::mat4& get_model_matrix() { return m_model_matrix; }

	virtual int get_quantity() const { return 1; }
	virtual float get_rotation_angle() const { return 0; }
	virtual const glm::vec3& get_step() const { return glm::vec3(0.0f); }

	virtual void set_in_space(const glm::vec3& position, float scale, float angle, const details_object& details_obj,  std::vector<glm::mat4>& model_matrices_vector) {}

protected:
	GLuint m_model_matrix_id;
	glm::mat4 m_model_matrix;

	GLuint m_mvp_matrix_id;
	glm::mat4 m_mvp_matrix;

	bool is_exists_noraml;
};

#endif