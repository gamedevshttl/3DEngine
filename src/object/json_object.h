#ifndef __custom_object__
#define __custom_object__

#include "object.h"
#include <vector>

class json_object: public object
{
public:
	virtual ~json_object();
	virtual void init(const std::string& path, shader_logic& shader);
	virtual void draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos);
	virtual void draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model);
	virtual std::shared_ptr<object> clone();

	void load_texture(texture& texture_item);

protected:
	std::string load_file(const std::string& path);

	std::vector<vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<texture> m_textures;

	//std::string m_diff_texture_path;

	GLuint m_VAO;
};

#endif