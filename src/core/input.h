#pragma once

#include "../math/vector3.h"

struct GLFWwindow;

namespace igad
{
	class Input
	{
	public:
		Input();
		~Input();

		static Input* Instance();
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		void Initialise(GLFWwindow* window);

		void Update();

		int GetKeyState(int key);
		bool IsKeyDown(int key);
		bool IsKeyRepeating(int key);
		bool IsKeyReleased(int key);

		const Vector2& GetMousePosition();
		const Vector2& GetMouseDelta();
		int GetMouseButtonState(int key);
		bool IsMouseButtonDown(int key);
		bool IsMouseButtonReleased(int key);
	private:
		GLFWwindow* window_;
		int key_states_[400];
		int mouse_states_[10];
		Vector2 mouse_position_;
		Vector2 newest_mouse_position_;
		Vector2 delta_mouse_position_;
	};
}