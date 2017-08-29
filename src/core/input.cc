#include "input.h"

#include "../graphics/glfw/glfw3.h"
#include "../memory/memory_manager.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Input::Input()
	{

	}

	//------------------------------------------------------------------------------------------------------
	Input::~Input()
	{

	}

	//------------------------------------------------------------------------------------------------------
	Input* Input::Instance()
	{
		static Input* device = nullptr;
		static memory::PoolAllocator* alloc = nullptr;

		if (device == nullptr)
		{
			memory::MemoryManager::Create();
			alloc = memory::MemoryManager::Instance()->ConstructAllocator<memory::PoolAllocator, Input>(sizeof(Input));
			device = memory::MemoryManager::Allocate<Input>(alloc);
		}

		return device;
	}

	//------------------------------------------------------------------------------------------------------
	void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Input::Instance()->key_states_[key] = action;
	}

	//------------------------------------------------------------------------------------------------------
	void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Input::Instance()->mouse_states_[button] = action;
	}

	//------------------------------------------------------------------------------------------------------
	void Input::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Input::Instance()->newest_mouse_position_ = Vector2{ static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	//------------------------------------------------------------------------------------------------------
	void Input::Initialise(GLFWwindow* window)
	{
		window_ = window;

		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
	}

	//------------------------------------------------------------------------------------------------------
	void Input::Update()
	{
		double x, y;
		glfwGetCursorPos(window_, &x, &y);

		delta_mouse_position_.x = x - mouse_position_.x;
		delta_mouse_position_.y = y - mouse_position_.y;

		mouse_position_.x = x;
		mouse_position_.y = y;
	}

	//------------------------------------------------------------------------------------------------------
	int Input::GetKeyState(int key)
	{
		return key_states_[key];
	}

	//------------------------------------------------------------------------------------------------------
	bool Input::IsKeyDown(int key)
	{
		return key_states_[key] == GLFW_PRESS || key_states_[key] == GLFW_REPEAT;
	}

	//------------------------------------------------------------------------------------------------------
	bool Input::IsKeyRepeating(int key)
	{
		return key_states_[key] == GLFW_REPEAT;
	}

	//------------------------------------------------------------------------------------------------------
	bool Input::IsKeyReleased(int key)
	{
		return key_states_[key] == GLFW_RELEASE;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector2& Input::GetMousePosition()
	{
		return newest_mouse_position_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector2& Input::GetMouseDelta()
	{
		return delta_mouse_position_;
	}
	
	//------------------------------------------------------------------------------------------------------
	int Input::GetMouseButtonState(int key)
	{
		return mouse_states_[key];
	}

	//------------------------------------------------------------------------------------------------------
	bool Input::IsMouseButtonDown(int key)
	{
		return mouse_states_[key] == GLFW_PRESS;
	}

	//------------------------------------------------------------------------------------------------------
	bool Input::IsMouseButtonReleased(int key)
	{
		return mouse_states_[key] == GLFW_RELEASE;
	}
}