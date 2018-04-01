#ifndef __scene__
#define __scene__

#include "camera.h"
#include "shader_logic.h"
#include "object\factory_object.h"

#include "box.h"
#include "object\model.h"
#include "light.h"
#include "post_processing.h"
#include "skybox.h"

#include <vector>
#include <memory>

struct GLFWwindow;
class object;

class scene
{
public:	
	void mouse_callback(double xpos, double ypos);
	void init(GLFWwindow* window);
	void draw();

private:
	void create_object();

	GLFWwindow* m_window;
	camera m_camera;
	//Camera tcamera;


	glm::mat4 projection;
	glm::mat4 view;

	shader_logic m_custom_object_shader;
	shader_logic m_border_custom_object_shader;
	std::vector<std::shared_ptr<object>> m_object_vector;

	factory_object factory;
	light m_light;

	std::vector<std::shared_ptr<object>> m_light_gebug_object_vector;
	shader_logic m_debug_light_shader;

	post_processing m_post_processing;
	skybox m_skybox;
};

#endif



