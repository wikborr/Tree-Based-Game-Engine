#pragma once

#include "servers/FileReader.h"

//does stretching window change the orthogonal matrix?
enum StretchMode{ 
	STRETCH_MODE_NOCHANGE,
	STRETCH_MODE_CHANGE
};

//global settings singleton
class Settings{
public:
	//switches to true just before the game loop
	bool initialized = false;

	//singleton method
	static Settings& ins();

	//attributes
	GLFWwindow* main_window = nullptr;
	bool fullscreen= false;
	int screenPosX = 0;
	int screenPosY = 0;
	int defaultGameWidth = 854;
	int defaultGameHeight = 480;
	int game_width = 854;
	int game_height = 480;
	int screen_width = 854;
	int screen_height = 480;
	std::string project_name = "Tree-Based Game Engine Test";
	std::string project_dir = "project/";
	std::string main_grass = "";
	glm::vec3 bg_color = glm::vec3(0.0f, 0.33f, 0.33f);
	GLFWimage icon[1];
	FT_Library ftLibrary;

	int limitFPS = 0;
	bool pixelSnap = false;
	StretchMode stretchMode = STRETCH_MODE_NOCHANGE;
	bool debugCollision = false;

	//methods
	std::string generateSettings(std::string project_dir);
	int createMainWindow();
	int initGLAD();

	void goFullscreen(); //goes into or exits the fullscreen
	void closeGame();
	
	//callback functions
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
