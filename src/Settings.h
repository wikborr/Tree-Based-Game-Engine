#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//global settings singleton
class Settings{
public:
	//singleton method
	static Settings& ins();

	//attributes
	GLFWwindow* main_window = nullptr;
	GLFWmonitor* fullscreen_monitor = nullptr;
	int screen_width = 854;
	int screen_height = 480;
	std::string screen_title = "Tree-Based Game Engine";
	
	glm::vec3 bg_color = glm::vec3(0.16f, 0.44f, 0.44f);


	//methods
	int createMainWindow(bool fullscreen = false);
	int initGLAD();
	
	//callback function
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
