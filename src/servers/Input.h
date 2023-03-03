#pragma once

#include "Settings.h"

const int KEY_NUMBER = 512;
const int MOUSE_NUMBER = 16;
const int JOYSTICK_NUMBER = 16;
const int JOYSTICK_BUTTON_NUMBER = 15;
const int JOYSTICK_AXIS_NUMBER = 6;
const float DEFAULT_JOY_DEADZONE = 0.1f;

enum MouseMode{
	MOUSE_MODE_VISIBLE,
	MOUSE_MODE_HIDDEN,
	MOUSE_MODE_CAPTURED,
};

//server singleton for handling user input
class Input{
public:
	//singleton method
	static Input& ins();

	MouseMode mouseMode = MOUSE_MODE_VISIBLE;
	std::array<std::pair<bool, bool>, KEY_NUMBER> keys;
	std::array<std::pair<bool, bool>, MOUSE_NUMBER> mouseKeys;
	glm::vec2 mousePos = glm::vec2(0.0f, 0.0f);
	glm::vec2 prevMousePos = glm::vec2(0.0f, 0.0f);
	glm::vec2 mouseScrollOffset = glm::vec2(0.0f, 0.0f);
	std::map<std::string, std::vector<int>> actions;

	struct Joystick{
		bool active = false;
		std::string name = "";
		GLFWgamepadstate state;
		std::array<std::pair<bool, bool>, JOYSTICK_BUTTON_NUMBER> buttons;
		std::array<std::pair<float, float>, JOYSTICK_AXIS_NUMBER> axes;
	};
	std::array<Joystick, JOYSTICK_NUMBER> joys;
	
	bool mouseOnScreen = false;
	bool windowFocused = false;

	Input();
	bool checkIfInputEvent();
	void removeJustKeys();
	void changeMouseMode(MouseMode newMouseMode = MOUSE_MODE_VISIBLE);
	glm::vec2 getMouseOffset();

	bool isKeyJustPressed(int key);
	bool isKeyPressed(int key);
	bool isKeyJustReleased(int key);
	bool isMouseJustPressed(int button);
	bool isMousePressed(int button);
	bool isMouseJustReleased(int button);
	bool isJoyButtonJustPressed(int button, int axis=0, float sensivity = DEFAULT_JOY_DEADZONE, int joy_number=-1);
	bool isJoyButtonPressed(int button, int axis=0, float sensivity = DEFAULT_JOY_DEADZONE, int joy_number=-1);
	bool isJoyButtonJustReleased(int button, int axis=0, float sensivity = DEFAULT_JOY_DEADZONE, int joy_number=-1);
	bool isActionJustPressed(std::string, int axis=0, float sensivity = DEFAULT_JOY_DEADZONE, int joy_number=-1);
	bool isActionPressed(std::string, int axis=0, float sensivity = DEFAULT_JOY_DEADZONE, int joy_number=-1);
	bool isActionJustReleased(std::string, int axis=0, float sensivity = DEFAULT_JOY_DEADZONE, int joy_number=-1);
	float getAxisValue(int axis, int joy_number=-1);
	glm::vec2 getScrollOffset();

	//callback functions
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void cursor_enter_callback(GLFWwindow* window, int entered);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void joystick_callback(int jid, int event);
	void drop_callback(GLFWwindow* window, int count, const char** paths);
	void character_callback(GLFWwindow* window, unsigned int codepoint);
	void window_focus_callback(GLFWwindow* window, int focused);
};