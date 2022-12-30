#include "servers/Input.h"

Input& Input::ins(){
	static Input instance;
	return instance;
}

Input::Input(){
	this->keys.fill(std::pair<bool, bool>(false,false));
	this->mouseKeys.fill(std::pair<bool, bool>(false,false));
	for(auto &i: this->joys){
		i.buttons.fill(std::pair<bool, bool>(false,false));
		i.axes.fill(std::pair<float, float>(0.0f,0.0f));
	}
	this->window_focus_callback(Settings::ins().main_window, glfwGetWindowAttrib(Settings::ins().main_window, GLFW_FOCUSED));
	this->cursor_enter_callback(Settings::ins().main_window, glfwGetWindowAttrib(Settings::ins().main_window, GLFW_HOVERED));
	for(int i = 0; i<this->joys.size(); i++){
		if(glfwJoystickIsGamepad(i)){
			this->joystick_callback(i, GLFW_CONNECTED);
		}
	}
	glfwSetKeyCallback(Settings::ins().main_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void{
		Input::ins().key_callback(window, key, scancode, action, mods);
	});
	glfwSetCursorPosCallback(Settings::ins().main_window,  [](GLFWwindow* window, double xpos, double ypos)->void{
		Input::ins().cursor_position_callback(window, xpos, ypos);
	});
	glfwSetCursorEnterCallback(Settings::ins().main_window,  [](GLFWwindow* window, int entered)->void{
		Input::ins().cursor_enter_callback(window, entered);
	});
	glfwSetMouseButtonCallback(Settings::ins().main_window,  [](GLFWwindow* window, int button, int action, int mods)->void{
		Input::ins().mouse_button_callback(window, button, action, mods);
	});
	glfwSetScrollCallback(Settings::ins().main_window,  [](GLFWwindow* window, double xoffset, double yoffset)->void{
		Input::ins().scroll_callback(window, xoffset, yoffset);
	});
	glfwSetJoystickCallback([](int jid, int event)->void{
		Input::ins().joystick_callback(jid, event);
	});
	glfwSetDropCallback(Settings::ins().main_window,  [](GLFWwindow* window, int count, const char** paths)->void{
		Input::ins().drop_callback(window, count, paths);
	});
	glfwSetCharCallback(Settings::ins().main_window, [](GLFWwindow* window, unsigned int codepoint)->void{
		Input::ins().character_callback(window, codepoint);
	});
	glfwSetWindowFocusCallback(Settings::ins().main_window, [](GLFWwindow* window, int focused)->void{
		Input::ins().window_focus_callback(window, focused);
	});
}

bool Input::checkIfInputEvent(){
	bool joyEvent = false;
	for(int i = 0; i<this->joys.size(); i++){
		if(this->joys.at(i).active && glfwGetGamepadState(i, &this->joys.at(i).state)){
			for(int j = 0; j<JOYSTICK_BUTTON_NUMBER; j++){
				if(this->joys.at(i).state.buttons[j]){
					this->joys.at(i).buttons.at(j).first = true;
					joyEvent = true;
				}
			}
			for(int j = 0; j<JOYSTICK_AXIS_NUMBER; j++){
				if(j<4){
					if(this->joys.at(i).state.axes[j] > DEFAULT_JOY_DEADZONE || this->joys.at(i).state.axes[j] < -DEFAULT_JOY_DEADZONE){
						this->joys.at(i).axes.at(j).second = this->joys.at(i).axes.at(j).first;
						this->joys.at(i).axes.at(j).first = this->joys.at(i).state.axes[j];
						joyEvent = true;
					}
				}
				else if(this->joys.at(i).state.axes[j] > DEFAULT_JOY_DEADZONE-1.0){
					this->joys.at(i).axes.at(j).second = this->joys.at(i).axes.at(j).first;
					this->joys.at(i).axes.at(j).first = this->joys.at(i).state.axes[j];
					joyEvent = true;
				}
			}
		}
	}
	if(joyEvent) return joyEvent;
	for(const auto& i: this->keys){
		if(i.first || i.second){
			return true;
		}
	}
	for(const auto& i: this->mouseKeys){
		if(i.first || i.second){
			return true;
		}
	}
	return false;
}
void Input::removeJustKeys(){
	for(auto& i: this->keys){
		if(i.first && !i.second){
			i.second = true;
		}
		else if(!i.first && i.second){
			i.second = false;
		}
	}
	for(auto& i: this->mouseKeys){
		if(i.first && !i.second){
			i.second = true;
		}
		else if(!i.first && i.second){
			i.second = false;
		}
	}
	this->mouseScrollOffset = glm::vec2(0.0f, 0.0f);
	for(auto& j: this->joys){
		for(auto& i: j.buttons){
			if(i.first && !i.second){
				i.second = true;
			}
			else if(!i.first && i.second){
				i.second = false;
			}
		}
		for(int i = 0; i<4; i++){

		}
		for(int i = 4; i<6; i++){
			
		}
	}
}

void Input::changeMouseMode(MouseMode newMouseMode){
	if(this->mouseMode != newMouseMode){
		if(newMouseMode == MOUSE_MODE_VISIBLE){
			glfwSetInputMode(Settings::ins().main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (glfwRawMouseMotionSupported())
    			glfwSetInputMode(Settings::ins().main_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
		else if(newMouseMode == MOUSE_MODE_HIDDEN){
			glfwSetInputMode(Settings::ins().main_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			if (glfwRawMouseMotionSupported())
    			glfwSetInputMode(Settings::ins().main_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
		else if(newMouseMode == MOUSE_MODE_CAPTURED){
			glfwSetInputMode(Settings::ins().main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (glfwRawMouseMotionSupported())
    			glfwSetInputMode(Settings::ins().main_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	}
}

bool Input::isKeyJustPressed(int key){
	if(key<0) return false;
	if(this->keys.at(key).first && !this->keys.at(key).second) return true;
	return false;
}
bool Input::isKeyPressed(int key){
	if(key<0) return false;
	if(this->keys.at(key).first) return true;
	return false;
}
bool Input::isKeyJustReleased(int key){
	if(key<0) return false;
	if(!this->keys.at(key).first && this->keys.at(key).second) return true;
	return false;
}
bool Input::isMouseJustPressed(int button){
	if(button<0) return false;
	if(this->mouseKeys.at(button).first && !this->mouseKeys.at(button).second) return true;
	return false;
}
bool Input::isMousePressed(int button){
	if(button<0) return false;
	if(this->mouseKeys.at(button).first) return true;
	return false;
}
bool Input::isMouseJustReleased(int button){
	if(button<0) return false;
	if(!this->mouseKeys.at(button).first && this->keys.at(button).second) return true;
	return false;
}
bool Input::isJoyButtonJustPressed(int button, int axis, float sensivity, int joy_number){
	if(button<0) return false;
	if(joy_number == -1){
		if(axis == 0){
			for(const auto& i: this->joys){
				if(i.active && (i.buttons.at(button).first && !i.buttons.at(button).second)) return true;
			}
		}
		else if(axis == -1){
			for(int i = 0; i<4; i++){
				
			}
		}
		else if(axis == 1){

		}
		else{

		}
	}
	else{

	}
	return false;
}
bool Input::isJoyButtonPressed(int button, int axis, float sensivity, int joy_number){
	if(button<0) return false;
	if(joy_number == -1){
		if(axis == 0){
			for(const auto& i: this->joys){
				if(i.active && (i.buttons.at(button).first)) return true;
			}
		}
		else if(axis == -1){
			for(int i = 0; i<4; i++){
				
			}
		}
		else if(axis == 1){

		}
		else{

		}
	}
	else{
		
	}
	return false;
}
bool Input::isJoyButtonJustReleased(int button, int axis, float sensivity, int joy_number){
	if(button<0) return false;
	if(joy_number == -1){
		if(axis == 0){
			for(const auto& i: this->joys){
				if(i.active && (!i.buttons.at(button).first && i.buttons.at(button).second)) return true;
			}
		}
		else if(axis == -1){
			for(int i = 0; i<4; i++){
				
			}
		}
		else if(axis == 1){

		}
		else{

		}
	}
	else{
		
	}
	return false;
}
bool Input::isActionJustPressed(std::string action, int axis, float sensivity, int joy_number){
	return false;
}
bool Input::isActionPressed(std::string action, int axis, float sensivity, int joy_number){
	return false;
}
bool Input::isActionJustReleased(std::string action, int axis, float sensivity, int joy_number){
	return false;
}

float Input::getAxisValue(int axis, int joy_number){
	if(joy_number == -1){
		for(const auto& i: this->joys){
			if(i.active){
				return i.axes.at(axis).first;
			}
		}
	}
	else{
		return this->joys.at(joy_number).axes.at(axis).first;
	}
	return 0.0f;
}
glm::vec2 Input::getScrollOffset(){
	return this->mouseScrollOffset;
}

glm::vec2 Input::getMouseOffset(){
	float xOffset = this->mousePos.x - this->prevMousePos.x;
    float yOffset = this->mousePos.y - this->prevMousePos.y;
	return glm::vec2(xOffset, yOffset);
}


//callbacks
void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(key<0) return;
	if(action == GLFW_PRESS){
		this->keys.at(key).first = true;
	}
	else if(action == GLFW_RELEASE){
		this->keys.at(key).first = false;
	}
}
void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
	this->prevMousePos = this->mousePos;
	this->mousePos = glm::vec2(xpos, ypos);
}
void Input::cursor_enter_callback(GLFWwindow* window, int entered){
	if(entered) this->mouseOnScreen = true;
	else this->mouseOnScreen = false;
}
void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	if(action == GLFW_PRESS){
		this->mouseKeys.at(button).first = true;
	}
	else if(action == GLFW_RELEASE){
		this->mouseKeys.at(button).first = false;
	}
}
void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	this->mouseScrollOffset = glm::vec2(xoffset, yoffset);
}
void Input::window_focus_callback(GLFWwindow* window, int focused){
	if(focused){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		this->prevMousePos = glm::vec2(xpos, ypos);
		this->windowFocused = true;
	}
	else{
		this->windowFocused = false;
	}
}
void Input::joystick_callback(int jid, int event){
	if(jid<0 || !glfwJoystickIsGamepad(jid)) return;
	if (event == GLFW_CONNECTED){
		this->joys.at(jid).name = glfwGetGamepadName(jid);
        this->joys.at(jid).active = true;
    }
    else if (event == GLFW_DISCONNECTED){
        this->joys.at(jid).active = false;
    }
}
void Input::drop_callback(GLFWwindow* window, int count, const char** paths){

}
void Input::character_callback(GLFWwindow* window, unsigned int codepoint){

}