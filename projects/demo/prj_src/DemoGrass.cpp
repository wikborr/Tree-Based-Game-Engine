#include "DemoGrass.h"

#include "leaves/leaves2D/Camera2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/Timer.h"
#include "leaves/Tween.h"

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

	cameraUp = Input::ins().isKeyPressed(GLFW_KEY_UP);
	cameraDown = Input::ins().isKeyPressed(GLFW_KEY_DOWN);
	cameraLeft = Input::ins().isKeyPressed(GLFW_KEY_LEFT);
	cameraRight = Input::ins().isKeyPressed(GLFW_KEY_RIGHT);

	speedUp = Input::ins().isKeyPressed(GLFW_KEY_LEFT_SHIFT);
	zoomIn = Input::ins().isKeyPressed(GLFW_KEY_O);
	zoomOut = Input::ins().isKeyPressed(GLFW_KEY_P);

	if(Input::ins().isKeyJustPressed(GLFW_KEY_F)){
		Settings::ins().goFullscreen();
	}
	if(Input::ins().isKeyJustPressed(GLFW_KEY_ESCAPE)){
		Settings::ins().closeGame();
	}

	if(Input::ins().isKeyJustPressed(GLFW_KEY_T)){
		std::cout<<"Timer started"<<std::endl;
		getChild<Timer>("timer1")->start();
	}
}

void DemoGrass::update(double delta){
	if(static_cast<Timer*>(getChild("timer1"))->timeout()){
		std::cout<<"Timer ended"<<std::endl;
		if(modulate.a > 0.0){
			getChild<Tween>("tween1")->propInterpolate(modulate.a, 1.0, 0.0, 1.0, TRANS_QUAD);
		}
		else{
			getChild<Tween>("tween1")->propInterpolate(modulate.a, 0.0, 1.0, 1.0, TRANS_QUAD);
		}
		std::cout<<"Tween started"<<std::endl;
	}
	if(getChild<Tween>("tween1")->completed()){
		std::cout<<"Tween ended"<<std::endl;
	}

	double curVelocity = velocity;
	if(speedUp) curVelocity *= 2;
	if(move_right) getChild<Sprite>("sprite1")->position.x += delta*curVelocity*10;
	else if(move_left) getChild<Sprite>("sprite1")->position.x -= delta*curVelocity*10;
	if(move_up) getChild<Sprite>("sprite1")->position.y += delta*curVelocity*10;
	else if(move_down) getChild<Sprite>("sprite1")->position.y -= delta*curVelocity*10;

	if(rotate_left) getChild<Sprite>("sprite1")->rotationDegrees += delta*curVelocity;
	else if(rotate_right) getChild<Sprite>("sprite1")->rotationDegrees -= delta*curVelocity;

	if(scale_up) getChild<Sprite>("sprite1")->scale += delta*curVelocity*0.1;
	else if(scale_down) getChild<Sprite>("sprite1")->scale -= delta*curVelocity*0.1;

	if(cameraRight) getChild("sprite1")->getChild<Camera2D>("camera")->offset.x += delta*curVelocity*10;
	else if(cameraLeft) getChild("sprite1")->getChild<Camera2D>("camera")->offset.x -= delta*curVelocity*10;
	if(cameraUp) getChild("sprite1")->getChild<Camera2D>("camera")->offset.y += delta*curVelocity*10;
	else if(cameraDown) getChild("sprite1")->getChild<Camera2D>("camera")->offset.y -= delta*curVelocity*10;
	if(zoomIn) getChild("sprite1")->getChild<Camera2D>("camera")->zoom -= delta*curVelocity*0.1;
	else if(zoomOut) getChild("sprite1")->getChild<Camera2D>("camera")->zoom += delta*curVelocity*0.1;

	//std::cout<<"FPS: "<<static_cast<int>(1/delta)<<std::endl;
}