#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string & filePath)
	: m_FilePath(filePath), m_RendererID(0) {

	ShaderSource shaderSrc = ParseShader("res/shaders/basic.shader");
	m_RendererID = CreateShader(shaderSrc.VertexShader, shaderSrc.FragmentShader);

}

Shader::~Shader() {

	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {

	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string & name, int slot) {
	GLCall(glUniform1i(GetUniformLocation(name), slot));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string & name) {

	//EXISTS
	if (m_UniformLocations.find(name) != m_UniformLocations.end()) {
		return m_UniformLocations[name];
	}

	//Doesnt exist
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1) {
		std::cout << "WARNING:: uniform location not found ::" << name << std::endl;
	}

	m_UniformLocations[name] = location;

	return location;
}


ShaderSource Shader::ParseShader(const std::string &filePath) {

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	ShaderType sType = ShaderType::NONE;
	std::ifstream stream(filePath);
	std::stringstream ss[2];
	std::string line;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) { // npos means invalid string
			if (line.find("vertex") != std::string::npos) {
				//set mode to vertex
				sType = ShaderType::VERTEX;
			} else if (line.find("fragment") != std::string::npos) {
				//set mode  to fragment
				sType = ShaderType::FRAGMENT;
			}
		} else {
			ss[(int)sType] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//ERROR DETECTION =>
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //i = integer,  v= vector/array

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // allocating a message array with dynamic size named length

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Faild to compile shader " << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0; // cant return -1, because its return type is unsigned int
	}

	return id;
}



unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

