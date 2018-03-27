#ifndef __camera__
#define __camera__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GLFWwindow;

class camera {
public:
	camera();

	void init_cursor_pos(double pos_x, double pos_y);
	void calc_front();
	void mouse_process(double pos_x, double pos_y);

	void render_proces(GLFWwindow* window);

	glm::mat4& get_view();
	glm::vec3& get_pos();
private:
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_camera_up;

	glm::vec3 m_pos;

	//float yaw = 90;
	//float pitch = -89;

	float m_horizontal_angle;
	float m_vertical_angle;

	float m_last_x;
	float m_last_y;
	float m_radius;

	float m_current_frame;

	float m_delta_time;
	float m_last_frame;

	glm::mat4 m_view;
};

#endif