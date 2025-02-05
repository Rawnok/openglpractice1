#pragma once

#include <string>
#include <unordered_map>

struct ShaderSource {
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader {

public:
	Shader(const std::string& filePath);
	~Shader();


	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string & name, int slot);

	//set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocations;

	ShaderSource ParseShader(const std::string &filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

	int GetUniformLocation(const std::string& name);
};