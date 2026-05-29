#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;

	void use() {
		glUseProgram(ID);
	}
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	Shader(const char* vertexPath, const char* fragmentPath) {

		// Reads code from file

		std::string vertex_code;
		std::string fragment_code;
		std::ifstream FragmentShaderFile;
		std::ifstream VertexShaderFile;

		VertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		FragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			VertexShaderFile.open(vertexPath);
			FragmentShaderFile.open(fragmentPath);
			std::stringstream vertex_shader_stream, fragment_shader_stream;

			vertex_shader_stream << VertexShaderFile.rdbuf();
			fragment_shader_stream << FragmentShaderFile.rdbuf();

			VertexShaderFile.close();
			FragmentShaderFile.close();

			vertex_code = vertex_shader_stream.str();
			fragment_code = fragment_shader_stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "SHADER ERROR: FILE NOT SUCCESSFULLY READ" << std::endl;
		}
		const char* vertex_shader_code = vertex_code.c_str();
		const char* fragment_shader_code = fragment_code.c_str();

		// Compile code
		unsigned int vertex, fragment;
		int success;
		char info_log[512];

		// compile vertex
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, info_log);
			std::cout << "SHADER ERROR: FAILED TO COMPILE VERTEX SHADER\n" << info_log;
		}

		// compile fragment
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, info_log);
			std::cout << "SHADER ERROR: FAILED TO COMPILE FRAGMENT SHADER\n" << info_log;
		}

		// create program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, info_log);
			std::cout << "SHADER PROGRAM ERROR: FAILED TO LINK SHADERS\n" << info_log;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

};


#endif // !SHADER_H