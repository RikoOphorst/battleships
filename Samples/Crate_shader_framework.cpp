#include "stdafx.h"
#include <OpenGL.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Vector3.h>
#include <Shader.h>
#include <Mesh.h>
#include <Texture.h>

using namespace std;
using namespace igad;

Shader* shader;
Mesh* crate_mesh;
Texture* crate_texture;
ShaderAttribute* position_attribute;
ShaderAttribute* normal_attribute;
ShaderAttribute* texture_attribute;
ShaderParameter* view_parameter;
ShaderParameter* model_parameter;
ShaderParameter* projection_parameter;
ShaderParameter* texture_parameter;
float rotation = 0.0f;
int width;
int height;

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
	shader = new Shader("./Assets/Basic.vsh", "./Assets/Basic.fsh");
	crate_texture = new Texture("./Assets/crate.jpg");
	
	position_attribute		= shader->GetAttribute("a_position");
	texture_attribute		= shader->GetAttribute("a_texture");
	normal_attribute		= shader->GetAttribute("a_normal");

	view_parameter			= shader->GetParameter("u_view");
	projection_parameter	= shader->GetParameter("u_projection");
	model_parameter			= shader->GetParameter("u_model");
	texture_parameter		= shader->GetParameter("u_texture");

	shader->Activate();

	float ratio = float(width) / float(height);
	auto model = Matrix44::CreateIdentity();
	model_parameter->SetValue(model);

	auto persp = Matrix44::CreatePerspective(igad::DegToRad(60), ratio, 0.01f, 5.0f);
	projection_parameter->SetValue(persp);

	crate_mesh = new Mesh();
	crate_mesh->Load("./Assets/Box.obj");
}

void shutdown()
{
	delete shader;
	delete crate_texture;
	delete crate_mesh;
}

void render()
{
	Matrix44 view = Matrix44::CreateLookAt(
		Vector3(2.0f, 2.0f, 2.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f));
	rotation += 0.01f;
	view = view * Matrix44::CreateRotateY(rotation);

	glEnable(GL_DEPTH_TEST);

	shader->Activate();

	auto vertices = crate_mesh->GetVertices();
	auto indices = crate_mesh->GetIndices();

	const void* firstPosition	= &vertices[0].Position;
	const void* firstNormal		= &vertices[0].Normal;
	const void* firstTexture	= &vertices[0].Texture;
	const void* firstIndex		= &indices[0];

	GLsizei size = sizeof(igad::VertexFormat);
	position_attribute->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, firstPosition);
	normal_attribute->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, firstNormal);
	texture_attribute->SetAttributePointer(2, GL_FLOAT, GL_FALSE, size, firstTexture);
	view_parameter->SetValue(view);
	texture_parameter->SetValue(*crate_texture);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, firstIndex);
	GL_GET_ERROR();
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

