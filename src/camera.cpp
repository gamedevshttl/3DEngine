#include "camera.h"
#include <GLFW\glfw3.h>

camera::camera()
	: m_horizontal_angle(90)
	, m_vertical_angle(-89)
	, m_radius(2)
	//, m_last_x(pos_x)
	//, m_last_y(pos_y)
	, m_front(glm::vec3(0, 0, -1))
	, m_pos(glm::vec3(30, 30, -30))
	, m_delta_time(0.0f)
	, m_last_frame(0.0f)
{
	calc_front();
}

void camera::init_cursor_pos(double pos_x, double pos_y)
{
	m_last_x = pos_x;
	m_last_y = pos_y;
}

void camera::calc_front()
{
	m_front.x = m_radius * sin(glm::radians(m_vertical_angle)) * cos(glm::radians(m_horizontal_angle));
	m_front.y = m_radius * cos(glm::radians(m_vertical_angle));
	m_front.z = m_radius * sin(glm::radians(m_vertical_angle)) * sin(glm::radians(m_horizontal_angle));

	m_right.x = m_radius * sin(glm::radians(m_vertical_angle)) * cos(glm::radians(m_horizontal_angle) - 3.14 / 2);
	m_right.y = m_radius * cos(glm::radians(m_vertical_angle));
	m_right.z = m_radius * sin(glm::radians(m_vertical_angle)) * sin(glm::radians(m_horizontal_angle) - 3.14 / 2);

	m_camera_up = glm::cross(m_right, m_front);

	m_view = glm::lookAt(m_pos, m_pos + m_front, glm::vec3(0, 1, 0));
}

void camera::mouse_process(double pos_x, double pos_y)
{
	float x_offset = pos_x - m_last_x;
	float y_offset = m_last_y - pos_y;
	m_last_x = pos_x;
	m_last_y = pos_y;

	float sensivity = 0.05;
	x_offset *= sensivity;
	y_offset *= sensivity;

	m_horizontal_angle += x_offset;
	m_vertical_angle += y_offset;

	calc_front();
}

void camera::render_proces(GLFWwindow* window)
{
	float current_frame = glfwGetTime();
	//m_delta_time = m_last_frame - current_frame;
	m_delta_time = current_frame - m_last_frame;
	m_last_frame = current_frame;

	float camera_speed = 2.5 * m_delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_pos += m_front * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_pos -= m_front * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_pos += m_right * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_pos -= m_right * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_pos.y += camera_speed;

	m_view = glm::lookAt(m_pos, m_pos + m_front, glm::vec3(0, 1, 0));
}

glm::mat4& camera::get_view() {
	return m_view;
}

glm::vec3& camera::get_pos() {
	return m_pos;
}
