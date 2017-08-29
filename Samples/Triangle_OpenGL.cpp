#include "stdafx.h"
#include <OpenGL.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Utils.h>
#include <Vector3.h>

using namespace std;
using namespace igad;

GLuint rendering_program;
GLint position_location;
GLint color_location;

struct Vertex
{
	Vector3 position;
	Vector3 color;
};

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint compile_shaders()
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	string vertex_shader_source_string = igad::ReadFile("./Assets/Triangle.vsh");
	static const GLchar* vertex_shader_source = vertex_shader_source_string.c_str();

	string fragment_shader_source_string = igad::ReadFile("./Assets/Triangle.fsh");
	static const GLchar* fragment_shader_source = fragment_shader_source_string.c_str();

	// Create and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GL_GET_ERROR();
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	GL_GET_ERROR();
	glCompileShader(vertex_shader);

	// Create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	GL_GET_ERROR();
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	GL_GET_ERROR();
	glCompileShader(fragment_shader);

	// Create program, attach shaders and link
	program = glCreateProgram();
	GL_GET_ERROR();
	glAttachShader(program, vertex_shader);
	GL_GET_ERROR();
	glAttachShader(program, fragment_shader);
	GL_GET_ERROR();
	glLinkProgram(program);
	GL_GET_ERROR();

	GLint logLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = static_cast<GLchar *>(malloc(logLength));
		glGetProgramInfoLog(program, logLength, &logLength, log);
		LOG("Program link log:\n%s", log);
		free(log);
	}

	glDeleteShader(vertex_shader);
	GL_GET_ERROR();
	glDeleteShader(fragment_shader);
	GL_GET_ERROR();

	return program;
}

void startup()
{
	rendering_program	= compile_shaders();
	position_location	= glGetAttribLocation(rendering_program, "a_position");
	color_location		= glGetAttribLocation(rendering_program, "a_color");
}

void render()
{
	static Vertex vertices[3] =
	{
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3(1.0f, 0.0f, 1.0f) },
		{ Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 0.0f) }
	};

	glUseProgram(rendering_program);

	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices[0].position.x);
	GL_GET_ERROR();
	glEnableVertexAttribArray(position_location);
	GL_GET_ERROR();

	glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices[0].color.x);
	GL_GET_ERROR();
	glEnableVertexAttribArray(color_location);
	GL_GET_ERROR();

	glDrawArrays(GL_TRIANGLES, 0, 3);
	GL_GET_ERROR();
}

void shutdown()
{
	glDeleteProgram(rendering_program);
}

int main(void)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		 // yes, 3 and 2!!!
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // But also 4 if present
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1280, 720, "Simple example", nullptr, nullptr);

	int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	cout << "OpenGL Version " << major << "." << minor << "." << revision << endl;

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		cout << "Failed to initialize OpenGL context" << endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	startup();

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, width, height);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

