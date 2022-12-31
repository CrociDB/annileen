#pragma once

#pragma warning( push )
// Suppressing strcpy warning
#pragma warning( disable : 4996)
#include <dear-imgui/imgui.h>
#pragma warning( pop )

#include <GLFW/glfw3.h>

#define ANNI_GLFW_KEY_TO_IMGUI(X) __getImGuiKeyFromGLFW(X)

inline ImGuiKey __getImGuiKeyFromGLFW(int glfwkey)
{
	switch (glfwkey)
	{
	case GLFW_KEY_TAB:
		return ImGuiKey_Tab;
	case GLFW_KEY_LEFT:
		return ImGuiKey_LeftArrow;
	case GLFW_KEY_RIGHT:
		return ImGuiKey_RightArrow;
	case GLFW_KEY_UP:
		return ImGuiKey_UpArrow;
	case GLFW_KEY_DOWN:
		return ImGuiKey_DownArrow;
	case GLFW_KEY_PAGE_UP:
		return ImGuiKey_PageUp;
	case GLFW_KEY_PAGE_DOWN:
		return ImGuiKey_PageDown;
	case GLFW_KEY_HOME:
		return ImGuiKey_Home;
	case GLFW_KEY_END:
		return ImGuiKey_End;
	case GLFW_KEY_INSERT:
		return ImGuiKey_Insert;
	case GLFW_KEY_DELETE:
		return ImGuiKey_Delete;
	case GLFW_KEY_BACKSPACE:
		return ImGuiKey_Backspace;
	case GLFW_KEY_SPACE:
		return ImGuiKey_Space;
	case GLFW_KEY_ENTER:
		return ImGuiKey_Enter;
	case GLFW_KEY_ESCAPE:
		return ImGuiKey_Escape;
	case GLFW_KEY_KP_ENTER:
		return ImGuiKey_KeypadEnter;

	case GLFW_KEY_LEFT_CONTROL:
		return ImGuiKey_LeftCtrl;
	case GLFW_KEY_RIGHT_CONTROL:
		return ImGuiKey_RightCtrl;
	case GLFW_KEY_LEFT_SHIFT:
		return ImGuiKey_LeftShift;
	case GLFW_KEY_RIGHT_SHIFT:
		return ImGuiKey_RightShift;
	case GLFW_KEY_LEFT_ALT:
		return ImGuiKey_LeftAlt;
	case GLFW_KEY_RIGHT_ALT:
		return ImGuiKey_RightAlt;
	case GLFW_KEY_LEFT_SUPER:
		return ImGuiKey_LeftSuper;
	case GLFW_KEY_RIGHT_SUPER:
		return ImGuiKey_RightSuper;

	case GLFW_KEY_A:
		return ImGuiKey_A;
	case GLFW_KEY_B:
		return ImGuiKey_B;
	case GLFW_KEY_C:
		return ImGuiKey_C;
	case GLFW_KEY_D:
		return ImGuiKey_D;
	case GLFW_KEY_E:
		return ImGuiKey_E;
	case GLFW_KEY_F:
		return ImGuiKey_F;
	case GLFW_KEY_G:
		return ImGuiKey_G;
	case GLFW_KEY_H:
		return ImGuiKey_H;
	case GLFW_KEY_I:
		return ImGuiKey_I;
	case GLFW_KEY_J:
		return ImGuiKey_J;
	case GLFW_KEY_K:
		return ImGuiKey_K;
	case GLFW_KEY_L:
		return ImGuiKey_L;
	case GLFW_KEY_M:
		return ImGuiKey_M;
	case GLFW_KEY_N:
		return ImGuiKey_N;
	case GLFW_KEY_O:
		return ImGuiKey_O;
	case GLFW_KEY_P:
		return ImGuiKey_P;
	case GLFW_KEY_Q:
		return ImGuiKey_Q;
	case GLFW_KEY_R:
		return ImGuiKey_R;
	case GLFW_KEY_S:
		return ImGuiKey_S;
	case GLFW_KEY_T:
		return ImGuiKey_T;
	case GLFW_KEY_U:
		return ImGuiKey_T;
	case GLFW_KEY_W:
		return ImGuiKey_W;
	case GLFW_KEY_X:
		return ImGuiKey_X;
	case GLFW_KEY_Y:
		return ImGuiKey_Y;
	case GLFW_KEY_Z:
		return ImGuiKey_Z;

	case GLFW_KEY_0:
		return ImGuiKey_0;
	case GLFW_KEY_1:
		return ImGuiKey_1;
	case GLFW_KEY_2:
		return ImGuiKey_2;
	case GLFW_KEY_3:
		return ImGuiKey_3;
	case GLFW_KEY_4:
		return ImGuiKey_4;
	case GLFW_KEY_5:
		return ImGuiKey_5;
	case GLFW_KEY_6:
		return ImGuiKey_6;
	case GLFW_KEY_7:
		return ImGuiKey_7;
	case GLFW_KEY_8:
		return ImGuiKey_8;
	case GLFW_KEY_9:
		return ImGuiKey_9;

	case GLFW_KEY_F1:
		return ImGuiKey_F1;
	case GLFW_KEY_F2:
		return ImGuiKey_F2;
	case GLFW_KEY_F3:
		return ImGuiKey_F3;
	case GLFW_KEY_F4:
		return ImGuiKey_F4;
	case GLFW_KEY_F5:
		return ImGuiKey_F5;
	case GLFW_KEY_F6:
		return ImGuiKey_F6;
	case GLFW_KEY_F7:
		return ImGuiKey_F7;
	case GLFW_KEY_F8:
		return ImGuiKey_F8;
	case GLFW_KEY_F9:
		return ImGuiKey_F9;
	case GLFW_KEY_F10:
		return ImGuiKey_F10;
	case GLFW_KEY_F11:
		return ImGuiKey_F11;
	case GLFW_KEY_F12:
		return ImGuiKey_F12;

	default:
		return 0;
	}
}