#include "stdafx.h"
#include <OpenGL.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Vector3.h>
#include <Shader.h>

using namespace std;
using namespace igad;

Shader* shader;
ShaderAttribute* position_attribute;
ShaderAttribute* color_attribute;

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

void startup()
{
	shader = new Shader("./Assets/Triangle.vsh", "./Assets/Triangle.fsh");
	position_attribute = shader->GetAttribute("a_position");
	color_attribute = shader->GetAttribute("a_color");
}

void render()
{
	static Vertex vertices[3] =
	{
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3(1.0f, 0.0f, 1.0f) },
		{ Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 0.0f) }
	};

	shader->Activate();
	position_attribute->SetAttributePointer(3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices[0].position.x);
	color_attribute->SetAttributePointer(3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices[0].color.x);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	GL_GET_ERROR();
}

void shutdown()
{
	delete shader;
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

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

