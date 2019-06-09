#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderSource {
	std::string VertexShader;
	std::string FragmentShader;
};

static ShaderSource ParseShader(const std::string &filePath) {

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

static unsigned int CompileShader(unsigned int type, const std::string& source) {

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



static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
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


int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float postions[] = {
			-0.1f, -0.1f,
			0.1f, -0.1f,
			0.1f, 0.1f,
			-0.1f, 0.1f,
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		VertexArray va;

		VertexBuffer vb(postions, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);
		ShaderSource shaderSrc = ParseShader("res/shaders/basic.shader");
		unsigned int shader = CreateShader(shaderSrc.VertexShader, shaderSrc.FragmentShader);
		GLCall(glUseProgram(shader));

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		GLCall(glUniform4f(location, 0.2f, 0.2f, 0.7f, 1.0f));

		//unbind everything
		va.UnBind();
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		// PROCEED TO DRAW CALL
		float r = 0.0f;
		float increment = 0.05f;

		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.2f, 0.7f, 1.0f));

			//attrib pointer function actually links the buffer then whichever it was attached to
			va.Bind();
			ib.Bind();

			//6 is the number equal to how many *indices* we want to draw
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f) {
				increment = -0.05f;
			} else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glDeleteProgram(shader);
	}
	glfwTerminate();
	return 0;
}