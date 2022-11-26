#include "Settings.h"

#include <sstream>
#include "servers/Renderer.h"
#include <stb_image.h>

//make utils class and move it there
void print(std::string line){
	std::cerr<<line<<std::endl;
}

Settings& Settings::ins(){
	static Settings instance;
	return instance;
}

std::string Settings::generateSettings(std::string project_dir){
	this->project_dir = project_dir;
	std::string settingsString = FileReader::ins().loadAsString(this->project_dir+"project.settings");
	if(settingsString == ""){
		return project_dir+": project.settings file not found!";
	}
	std::istringstream f(settingsString);
	std::string line;
	std::string property;
	while (std::getline(f, line)){
		if(line.find(property = "project_name: ") != std::string::npos){
			this->project_name = line.substr(property.length());
		}
		else if(line.find(property = "main_grass: ") != std::string::npos){
			this->main_grass = line.substr(property.length());
		}
		else if(line.find(property = "screen_width: ") != std::string::npos){
			this->screen_width = std::stoi(line.substr(property.length()));
			this->game_width = std::stoi(line.substr(property.length()));
		}
		else if(line.find(property = "screen_height: ") != std::string::npos){
			this->screen_height = std::stoi(line.substr(property.length()));
			this->game_height = std::stoi(line.substr(property.length()));
		}
		else if(line.find(property = "fullscreen: ") != std::string::npos){
			this->fullscreen = (line.substr(property.length()) == "0") ? false : true;
		}
	}
	if(this->main_grass == ""){
		return project_dir+": main_grass not specified.";
	}
	return "";
}

int Settings::createMainWindow(){
	this->main_window = glfwCreateWindow(this->screen_width, this->screen_height, project_name.c_str(), nullptr, nullptr);
	if(this->main_window == nullptr){
		return -1;
	}
	glfwMakeContextCurrent(this->main_window);
	std::string icoPath = project_dir+"icon.png";
	if(FileReader::ins().checkIfFileExists(icoPath)){
		this->icon[0].pixels = stbi_load(icoPath.c_str(), &this->icon[0].width, &this->icon[0].height, 0, 4);
		glfwSetWindowIcon(this->main_window, 1, this->icon); 
		stbi_image_free(this->icon[0].pixels);
	}
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
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;
}

void Settings::goFullscreen(){
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(this->fullscreen){
		glfwSetWindowMonitor(this->main_window, nullptr, this->screenPosX, this->screenPosY, this->screen_width, this->screen_height, GLFW_DONT_CARE);
		this->fullscreen = false;
	}
	else{
		this->fullscreen = true;
		glfwGetWindowPos(this->main_window, &this->screenPosX, &this->screenPosY);
		glfwSetWindowMonitor(this->main_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	}
}

//callbacks
void Settings::framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	if(!fullscreen){
		this->screen_width = width;
		this->screen_height = height;
	}
	if(this->stretchMode == STRETCH_MODE_CHANGE){
		this->game_width = width;
		this->game_height = height;
	}
	if(this->initialized){
		Renderer::ins().render();
	}
}