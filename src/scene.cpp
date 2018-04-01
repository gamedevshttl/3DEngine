#include "scene.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include <random>
#include <iostream>

#include "camera.h"
//#include "camera_tutor.h"
#include "texture_logic.h"

#include "object/json_object.h"

// settings
const unsigned int scene_SCR_WIDTH = 1024;
const unsigned int scene_SCR_HEIGHT = 768;
//
//float scene_lastX = (float)scene_SCR_WIDTH / 2.0;
//float scene_lastY = (float)scene_SCR_HEIGHT / 2.0;
//bool scene_firstMouse = true;
//
//// timing
//float scene_deltaTime = 0.0f;
//float scene_lastFrame = 0.0f;

void scene::mouse_callback(double xpos, double ypos)
{
	m_camera.mouse_process(xpos, ypos);


	//if (scene_firstMouse)
	//{
	//	scene_lastX = xpos;
	//	scene_lastY = ypos;
	//	scene_firstMouse = false;
	//}

	//float xoffset = xpos - scene_lastX;
	//float yoffset = scene_lastY - ypos; // reversed since y-coordinates go from bottom to top

	//scene_lastX = xpos;
	//scene_lastY = ypos;

	//tcamera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void scene::init(GLFWwindow* window)
{
	m_window = window;	
	glClearColor(0.0, 0.0f, 0.4f, 0.0f);

	//tcamera.Position = (glm::vec3(0.0f, 0.0f, 3.0f));

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	create_object();
}

void scene::create_object()
{	
	{
		factory.parce_scene("../resources/objects_data/scene.json");
	}

	//{
	//	m_custom_object_shader.load_shader("../src/shader/box_vertex_shader.glsl", "../src/shader/box_fragment_shader.glsl");
	//	m_border_custom_object_shader.load_shader("../src/shader/box_vertex_shader.glsl", "../src/shader/border_color.fs");

	//	//std::shared_ptr<object> plane = factory.create_object("../resources/objects_data/plane_data.json", m_custom_object_shader);

	//	//if(plane)
	//	//	m_object_vector.push_back(plane);
	//}

	//{
	//	std::shared_ptr<object> box = factory.create_object("../resources/objects_data/box_data.json", m_custom_object_shader);

	//	if (box) {
	//		box->set_position(glm::vec3(1.0f, 1.f, 1.f));
	//		m_object_vector.push_back(box);
	//	}

	//	for(int i = 1; i<10; ++i)
	//	{
	//		std::shared_ptr<object> obj_box =box->clone();
	//		if (obj_box) {
	//			obj_box->set_position(glm::vec3(i, i, i));
	//			m_object_vector.push_back(obj_box);
	//		}
	//	}
	//}

	{
		m_light.create_light("../resources/objects_data/light.json");

		m_debug_light_shader.load_shader("../src/shader/vertex_light_shader.glsl", "../src/shader/fragment_ligth_shader.glsl");

		for (auto elem : m_light.get_point_light_vetor()) {
			std::shared_ptr<object> light_gebug_object = factory.create_object("../resources/objects_data/box_data.json", m_debug_light_shader);
			light_gebug_object->set_position(elem.m_position);
			m_light_gebug_object_vector.push_back(light_gebug_object);			
		}

		
		m_light.set_uniform_light(m_custom_object_shader);
		//m_light.set_uniform_light(m_border_custom_object_shader);

		for (auto &elem : factory.get_map_vector_object())
			m_light.set_uniform_light(*elem.second.m_shader.get());
	}



	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);
	m_camera.init_cursor_pos(xpos, ypos);

	projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
	view = m_camera.get_view();	
	
	//glEnable(GL_CULL_FACE);	
	//glFrontFace(GL_CCW);

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	GLuint quadVBO;
	glGenVertexArrays(1, &m_quadVAO);
	glBindVertexArray(m_quadVAO);

	glGenBuffers(1, &quadVBO);	
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//glGenVertexArrays(1, &m_quadVAO);
	//glGenBuffers(1, &quadVBO);
	//glBindVertexArray(m_quadVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	m_framebuffers_shader.load_shader("../src/shader/1.framebuffers_screen.vs", "../src/shader/1.framebuffers_screen.fs");
	m_framebuffers_shader.use();
	m_framebuffers_shader.set_int("screenTexture", 0);


	// framebuffer configuration
	// -------------------------
	//unsigned int framebuffer;
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	// create a color attachment texture
	//unsigned int textureColorbuffer;
	glGenTextures(1, &m_texture_color_buffer);
	glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene_SCR_WIDTH, scene_SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_color_buffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scene_SCR_WIDTH, scene_SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glDepthFunc(GL_LESS);

	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void scene::draw()
{
	do {
		float time = glfwGetTime();

		//light_pos = glm::vec3(10.0f * sin(time), 1.0f, 10.0f * cos(time));		

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);		
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_camera.render_proces(m_window);
		view = m_camera.get_view();

		//view = tcamera.GetViewMatrix();
		//projection = glm::perspective(glm::radians(tcamera.Zoom), (float)scene_SCR_WIDTH / (float)scene_SCR_HEIGHT, 0.1f, 100.0f);

		//glStencilMask(0x00);

		//shrd_logic.use();
		//glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);		
		//shrd_logic.set_vec3("point_light_array[4].position", light_pos);

		glm::vec3 view_pos = m_camera.get_pos();

		{			
			for(const auto& elem : m_light_gebug_object_vector)
				elem->draw(m_debug_light_shader, projection, view, view_pos);
		}

		for (auto &elem_shader : factory.get_map_vector_object())
			for (auto &object : elem_shader.second.m_vector_object) {
				object->draw(*elem_shader.second.m_shader.get(), projection, view, view_pos);
			}


		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
								  // clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		m_framebuffers_shader.use();
		glBindVertexArray(m_quadVAO);
		glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);
}
