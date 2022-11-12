#include "Settings.h"

Settings& Settings::ins(){
	static Settings instance;
	return instance;
}

int Settings::createMainWindow(bool fullscreen){
	if(fullscreen){
		this->fullscreen_monitor = glfwGetPrimaryMonitor();
	}
	this->main_window = glfwCreateWindow(this->screen_width, this->screen_height, screen_title.c_str(), this->fullscreen_monitor, nullptr);
	if(this->main_window == nullptr){
		return -1;
	}
	glfwMakeContextCurrent(this->main_window);
	return 0;
}

int Settings::initGLAD(){
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		return -1;
	}
	glViewport(0, 0, this->screen_width, this->screen_height);
	glfwSetFramebufferSizeCallback(this->main_window, [](GLFWwindow* window, int width, int height)->void{
		Settings::ins().framebuffer_size_callback(window, width, height);
	});
	return 0;
}

void Settings::framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	this->screen_width = width;
	this->screen_height = height;
}