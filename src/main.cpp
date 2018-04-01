#include <iostream>

#define GLEW_STATIC
#include <GL\glew.h>

#include <GLFW\glfw3.h>
GLFWwindow* window;

#include "scene.h"

scene main_scene;

//#include "camera_tutor.h"
//#include "texture_logic.h"
//#include "shader_logic.h"

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow *window);


//// settings
//const unsigned int SCR_WIDTH = 1280;
//const unsigned int SCR_HEIGHT = 720;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = (float)SCR_WIDTH / 2.0;
//float lastY = (float)SCR_HEIGHT / 2.0;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	main_scene.mouse_callback(xpos, ypos);
	//cmra.mouse_process(xpos, ypos);	
}

int main(int argc, char** argv)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	main_scene.init(window);
	main_scene.draw();

	//glEnable(GL_DEPTH_TEST);

	//// build and compile shaders
	//// -------------------------
	//shader_logic shader, screenShader;
	//shader.load_shader("../src/shader/5.1.framebuffers.vs", "../src/shader/5.1.framebuffers.fs");
	//screenShader.load_shader("../src/shader/1.framebuffers_screen.vs", "../src/shader/1.framebuffers_screen.fs");

	//// set up vertex data (and buffer(s)) and configure vertex attributes
	//// ------------------------------------------------------------------
	//float cubeVertices[] = {
	//	// positions          // texture Coords
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	//};
	//float planeVertices[] = {
	//	// positions          // texture Coords 
	//	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	//	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	//	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	//	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	//	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	//	5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	//};
	//float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	//						 // positions   // texCoords
	//	-1.0f,  1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f,  0.0f, 0.0f,
	//	1.0f, -1.0f,  1.0f, 0.0f,

	//	-1.0f,  1.0f,  0.0f, 1.0f,
	//	1.0f, -1.0f,  1.0f, 0.0f,
	//	1.0f,  1.0f,  1.0f, 1.0f
	//};
	//// cube VAO
	//unsigned int cubeVAO, cubeVBO;
	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &cubeVBO);
	//glBindVertexArray(cubeVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//// plane VAO
	//unsigned int planeVAO, planeVBO;
	//glGenVertexArrays(1, &planeVAO);
	//glGenBuffers(1, &planeVBO);
	//glBindVertexArray(planeVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//// screen quad VAO
	//unsigned int quadVAO, quadVBO;
	//glGenVertexArrays(1, &quadVAO);
	//glGenBuffers(1, &quadVBO);
	//glBindVertexArray(quadVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//// load textures
	//// -------------
	//texture_logic txtr_logic;
	//unsigned int cubeTexture = txtr_logic.load_image("../resources/texture/marble.jpg");
	//unsigned int floorTexture = txtr_logic.load_image("../resources/texture/metal.png");

	//// shader configuration
	//// --------------------
	//shader.use();
	//shader.set_int("texture1", 0);

	//screenShader.use();
	//screenShader.set_int("screenTexture", 0);

	//// framebuffer configuration
	//// -------------------------
	//unsigned int framebuffer;
	//glGenFramebuffers(1, &framebuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//// create a color attachment texture
	//unsigned int textureColorbuffer;
	//glGenTextures(1, &textureColorbuffer);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	//// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	//																							  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// draw as wireframe
	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//// render loop
	//// -----------
	//while (!glfwWindowShouldClose(window))
	//{
	//	// per-frame time logic
	//	// --------------------
	//	float currentFrame = glfwGetTime();
	//	deltaTime = currentFrame - lastFrame;
	//	lastFrame = currentFrame;

	//	// input
	//	// -----
	//	processInput(window);


	//	// render
	//	// ------
	//	// bind to framebuffer and draw scene as we normally would to color texture 
	//	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	//							 // make sure we clear the framebuffer's content
	//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	shader.use();
	//	glm::mat4 model;
	//	glm::mat4 view = camera.GetViewMatrix();
	//	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//	shader.set_mat4("view", view);
	//	shader.set_mat4("projection", projection);
	//	// cubes
	//	glBindVertexArray(cubeVAO);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	//	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	//	shader.set_mat4("model", model);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//	model = glm::mat4();
	//	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	//	shader.set_mat4("model", model);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//	// floor
	//	glBindVertexArray(planeVAO);
	//	glBindTexture(GL_TEXTURE_2D, floorTexture);
	//	shader.set_mat4("model", glm::mat4());
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glBindVertexArray(0);

	//	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	//							  // clear all relevant buffers
	//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	screenShader.use();
	//	glBindVertexArray(quadVAO);
	//	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	//	glDrawArrays(GL_TRIANGLES, 0, 6);


	//	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	//	// -------------------------------------------------------------------------------
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

	//// optional: de-allocate all resources once they've outlived their purpose:
	//// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &planeVAO);
	//glDeleteVertexArrays(1, &quadVAO);
	//glDeleteBuffers(1, &cubeVBO);
	//glDeleteBuffers(1, &planeVBO);
	//glDeleteBuffers(1, &quadVBO);

	//glfwTerminate();

	return 0;
}