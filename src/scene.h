#ifndef __scene__
#define __scene__

#include "camera.h"
//#include "camera_tutor.h"
#include "shader_logic.h"
#include "object\factory_object.h"

#include "box.h"
#include "object\model.h"
#include "light.h"

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

	GLuint m_framebuffer;
	GLuint m_quadVAO;
	GLuint m_texture_color_buffer;
	shader_logic m_framebuffers_shader;

	unsigned int m_cubeVAO, m_planeVAO;
	shader_logic m_object_shader;

	GLuint cubeTexture;
	GLuint floorTexture;
};

#endif


