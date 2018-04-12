#include "mesh.h"

#include <assimp\scene.h>

class model : public object
{
public:	

	model() 
		: object()
		, m_instance_amount(0)
	{}

	//virtual void init(const std::string& path, shader_logic& shader);
	virtual void init(const std::string& path, shader_logic& shader, const details_object& details_obj = details_object());
	virtual void add_instance_matrix(GLuint amount, const glm::mat4& model);
	virtual void add_instance_matrix_vector(const std::vector<glm::mat4>& matrix_vector);
	//void set_position(const glm::vec3& position);
	//void set_rotation(float angle, const glm::vec3& vec);
	void post_init();
	
	void load_texture(texture& texture_item);

	virtual void draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos);
	//void draw(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view);
	void draw_instance(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos);
	virtual void draw_cust_model_matrix(shader_logic& shader, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& view_pos, const glm::mat4& model);
	void set_uniform_data(shader_logic& shader);

	virtual std::shared_ptr<object> clone();

private:
	void load_model(std::string path);
	void process_node(aiNode *node, const aiScene *scene);
	mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);	

	std::vector<mesh> m_meshes;
	std::string m_directory;

	std::vector<texture> m_textures_loaded;

	//GLuint m_model_matrix_id;
	//glm::mat4 m_model_matrix;

	//GLuint m_mvp_matrix_id;
	//glm::mat4 m_mvp_matrix;		

	std::vector<texture> m_textures;
	int m_instance_amount;
	std::vector<glm::mat4> m_instance_matrix_vector;
};