#include "shader_logic.h"
#include <fstream>
#include <vector>
#include <iostream>

std::string shader_logic::load_file(const std::string& path)
{
	std::string code;
	std::ifstream stream(path.c_str(), std::ios::in);
	if (stream) {
		std::string line;
		while (std::getline(stream, line))
			code += "\n" + line;
		code += "\n";
	}

	return code;
}

void shader_logic::compile_shader(GLuint shader_id, const std::string& shader_code) {
	GLint result;
	int info_log_lenght;

	char const* vertex_source_pointer = shader_code.c_str();
	glShaderSource(shader_id, 1, &vertex_source_pointer, NULL);
	glCompileShader(shader_id);

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_lenght);
	std::vector<char> vertex_shader_info_message(info_log_lenght);
	if (vertex_shader_info_message.size() > 0) {
		glGetShaderInfoLog(shader_id, info_log_lenght, NULL, &vertex_shader_info_message[0]);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: ";
		for (char symb : vertex_shader_info_message)
			std::cout << symb;
	}
}

GLuint shader_logic::load_shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertex_shader_code = load_file(vertex_shader_path);
	std::string fragment_shader_code = load_file(fragment_shader_path);

	compile_shader(vertex_shader_id, vertex_shader_code);
	compile_shader(fragment_shader_id, fragment_shader_code);
	
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	GLint result;
	int info_log_lenght;

	glGetProgramiv(program_id, GL_COMPILE_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_lenght);
	std::vector<char> programm_error_message(info_log_lenght);
	if (programm_error_message.size() > 0) {
		glGetShaderInfoLog(vertex_shader_id, info_log_lenght, NULL, &programm_error_message[0]);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: ";
		for (char symb : programm_error_message)
			std::cout << symb;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

void shader_logic::use()
{
	glUseProgram(program_id);
}

void shader_logic::set_bool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void shader_logic::set_int(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void shader_logic::set_float(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void shader_logic::set_vec3(const std::string& name, float a, float b, float c)
{
	glUniform3f(glGetUniformLocation(program_id, name.c_str()), a, b, c);
}

void shader_logic::set_vec3(const std::string& name, const glm::vec3& vec)
{
	glUniform3f(glGetUniformLocation(program_id, name.c_str()), vec.x, vec.y, vec.z);
}

void shader_logic::set_mat4(const std::string &name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

GLuint shader_logic::get_id() const
{
	return program_id;
}