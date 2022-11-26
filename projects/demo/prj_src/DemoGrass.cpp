#include "DemoGrass.h"

void DemoGrass::ready(){
	std::cout<<"DemoGrass loaded!"<<std::endl;
}

void DemoGrass::inputEvent(){
	move_up = Input::ins().isKeyPressed(GLFW_KEY_W);
	move_down = Input::ins().isKeyPressed(GLFW_KEY_S);
	move_right = Input::ins().isKeyPressed(GLFW_KEY_D);
	move_left = Input::ins().isKeyPressed(GLFW_KEY_A);
	rotate_left = Input::ins().isKeyPressed(GLFW_KEY_Q);
	rotate_right = Input::ins().isKeyPressed(GLFW_KEY_E);
	scale_up = Input::ins().isMousePressed(GLFW_MOUSE_BUTTON_1);
	scale_down = Input::ins().isMousePressed(GLFW_MOUSE_BUTTON_2);

	if(Input::ins().isKeyJustPressed(GLFW_KEY_F)){
		Settings::ins().goFullscreen();
	}
}

void DemoGrass::update(double delta){
	if(move_right) position.x += delta*velocity*10;
	else if(move_left) position.x -= delta*velocity*10;
	if(move_up) position.y += delta*velocity*10;
	else if(move_down) position.y -= delta*velocity*10;

	if(rotate_left) rotationDegrees += delta*velocity;
	else if(rotate_right) rotationDegrees -= delta*velocity;

	if(scale_up) scale += delta*velocity*0.1;
	else if(scale_down) scale -= delta*velocity*0.1;

	std::cout<<"FPS: "<<static_cast<int>(1/delta)<<std::endl;
}