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

void scene::mouse_callback(double xpos, double ypos)
{
	m_camera.mouse_process(xpos, ypos);
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
		m_rock_shader.load_shader("../src/shader/1.instancing.vs", "../src/shader/1.instancing.fs");
		m_rock.init("../resources/rock/rock.obj", m_rock_shader);

		unsigned int amount = 1000;		
		m_model_matrices = new glm::mat4[amount];
		srand(glfwGetTime()); // initialize random seed	
		float radius = 50.0;
		float offset = 2.5f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model;
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			m_model_matrices[i] = model;

			m_model_matrices_vector.push_back(model);
		}

		//m_rock.add_instance_matrix(amount, m_model_matrices[0]);
		m_rock.add_instance_matrix(amount, m_model_matrices_vector[0]);
	}

	{
		m_light.create_light("../resources/objects_data/light.json");

		m_debug_light_shader.load_shader("../src/shader/vertex_light_shader.glsl", "../src/shader/fragment_ligth_shader.glsl");

		for (auto elem : m_light.get_point_light_vetor()) {
			std::shared_ptr<object> light_gebug_object = factory.create_object("../resources/objects_data/box_data_cw.json", m_debug_light_shader);
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
	
	glEnable(GL_CULL_FACE);	
	//glFrontFace(GL_CCW);

	m_post_processing.init();
	m_skybox.init();

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

		m_post_processing.pre_draw();

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

		//m_skybox.draw(projection, view);

		for (auto &elem_shader : factory.get_map_vector_object())
			for (auto &object : elem_shader.second.m_vector_object) {
				object->draw(*elem_shader.second.m_shader.get(), projection, view, view_pos);
			}

		//for (unsigned int i = 0; i < 10000; i++)
		//{			
		//	m_rock.draw_cust_model_matrix(m_rock_shader, projection, view, view_pos, m_model_matrices[i]);
		//}

		//m_rock.draw_instance(m_rock_shader, projection, view, view_pos);

		m_rock.draw(m_rock_shader, projection, view, view_pos);

		//m_skybox.draw(projection, view);

		m_post_processing.draw();

		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);
}
