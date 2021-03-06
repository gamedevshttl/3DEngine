#ifndef __custom_object__
#define __custom_object__

#include "object.h"
#include <vector>

class json_object: public object
{
public:
	json_object() 
		: object()
		, m_instance_amount(0)
		, m_shadow(true)
		, m_quantity(1)
		, m_rotation_angle(0)
	{}

	virtual ~json_object();
	virtual void init(const std::string& path, shader_logic& shader, const details_object& details_obj = details_object());
	virtual void add_instance_matrix(GLuint amount, const glm::mat4& model);
	virtual void add_instance_matrix_vector(const std::vector<glm::mat4>& matrix_vector);
	virtual void post_init();
	virtual void draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, GLuint shadow_map = 0);
	virtual void draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model);
	virtual void draw_instance(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos);
	virtual void draw_shadow(shader_logic& shader);
	virtual std::shared_ptr<object> clone();

	void load_texture(texture& texture_item);
	void load_cube_texture_reflect(texture& texture_item);

	virtual int get_quantity() const;
	virtual float get_rotation_angle() const;
	virtual const glm::vec3& get_step() const;

	virtual void set_in_space(const glm::vec3& position, float scale, float angle, const details_object& details_obj,  std::vector<glm::mat4>& model_matrices_vector);

protected:
	std::string load_file(const std::string& path);

	std::vector<vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<texture> m_textures;

	bool m_shadow;

	GLuint m_VAO;
	GLuint m_instance_amount;

	std::vector<glm::mat4> m_instance_matrix_vector;

	int m_quantity;
	float m_rotation_angle;
	glm::vec3 m_step;
};

#endif