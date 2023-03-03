#include "MainCharacter.h"

#include "leaves/leaves2D/Camera2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/Timer.h"
#include "leaves/Tween.h"
#include "leaves/leaves2D/Area2D.h"
#include "leaves/leaves2D/KinematicBody2D.h"
#include "leaves/leaves2D/ParticleEmitter.h"

void MainCharacter::ready(){
}

void MainCharacter::inputEvent(){
	move_up = Input::ins().isKeyPressed(GLFW_KEY_W) || Input::ins().isKeyPressed(GLFW_KEY_SPACE);
	move_down = Input::ins().isKeyPressed(GLFW_KEY_S);
	move_right = Input::ins().isKeyPressed(GLFW_KEY_D);
	move_left = Input::ins().isKeyPressed(GLFW_KEY_A);
	rotate_left = Input::ins().isKeyPressed(GLFW_KEY_Q);
	rotate_right = Input::ins().isKeyPressed(GLFW_KEY_E);

	cameraUp = Input::ins().isKeyPressed(GLFW_KEY_UP);
	cameraDown = Input::ins().isKeyPressed(GLFW_KEY_DOWN);
	cameraLeft = Input::ins().isKeyPressed(GLFW_KEY_LEFT);
	cameraRight = Input::ins().isKeyPressed(GLFW_KEY_RIGHT);

	speedUp = Input::ins().isKeyPressed(GLFW_KEY_LEFT_SHIFT);
}

void MainCharacter::update(double delta){
	//tweens
	if(getChild<Tween>("tweenG")->completed()){
		if(getChild<Sprite>("sprite1")->modulate.g < 1.0){
			getChild<Tween>("tweenG")->reset(true);
			getChild<Tween>("tweenB")->reset(true);
			getChild<Tween>("tweenG")->propInterpolate(getChild<Sprite>("sprite1")->modulate.g, 0.4, 1.0, 0.25, TRANS_QUAD);
			getChild<Tween>("tweenB")->propInterpolate(getChild<Sprite>("sprite1")->modulate.b, 0.4, 1.0, 0.25, TRANS_QUAD);
		}
	}
	if(getChild<Timer>("inputTimer")->timeout()){
		this->noInput = false;
	}
	
	if(!noInput){
		velocity.x = 0;
	}
	double curVelocity = moveVelocity;
	if(!noInput){
		if(speedUp) curVelocity *= 3;
		if(move_right){
			scale.x = glm::abs(scale.x);
			velocity.x += delta*curVelocity*10;
		}
		else if(move_left){
			scale.x = -glm::abs(scale.x);
			velocity.x -= delta*curVelocity*10;
		}

		if(isOnFloor()){
			jumping = false;
			if(move_up){
				velocity.y = delta*jumpVelocity;
				jumping = true;
			}
		}
		if(velocity.y < -50){
			velocity.y = -50;
		}
		else if(velocity.y > 50){
			velocity.y = 50;
		}
	}
	
	velocity.y -= gravity;

	this->moveAndCollide(velocity);

	if(rotate_left) this->rotationDegrees += delta*curVelocity;
	else if(rotate_right) this->rotationDegrees -= delta*curVelocity;

	if(cameraRight) getChild<Camera2D>("camera")->offset.x += delta*curVelocity*10;
	else if(cameraLeft) getChild<Camera2D>("camera")->offset.x -= delta*curVelocity*10;
	if(cameraUp) getChild<Camera2D>("camera")->offset.y += delta*curVelocity*10;
	else if(cameraDown) getChild<Camera2D>("camera")->offset.y -= delta*curVelocity*10;

	//sprite animation
	if(walkingAnimFrames == 0 && (move_right || move_left)){
		walkingAnimFrames = 1;
	}
	else if(walkingAnimFrames != 0 && !(move_right || move_left)){
		walkingAnimFrames = 0;
	}
	if(jumping){
		getChild<Sprite>("sprite1")->sheetFrame = 3;
	}
	else if(walkingAnimFrames != 0){
		if(walkingAnimFrames > 19){
			walkingAnimFrames = 1;
		}

		if(walkingAnimFrames > 14){
			getChild<Sprite>("sprite1")->sheetFrame = 1;
		}
		else if(walkingAnimFrames > 9){
			getChild<Sprite>("sprite1")->sheetFrame = 2;
		}
		else if(walkingAnimFrames > 4){
			getChild<Sprite>("sprite1")->sheetFrame = 1;
		}
		else if(walkingAnimFrames > 0){
			getChild<Sprite>("sprite1")->sheetFrame = 0;
		}
		walkingAnimFrames +=1;
	}
	else{
		getChild<Sprite>("sprite1")->sheetFrame = 0;
	}
}

void MainCharacter::collision(){
	velocity = glm::vec2(-30, 50);
	getChild<Timer>("inputTimer")->start();
	this->noInput = true;
	getChild<Tween>("tweenG")->propInterpolate(getChild<Sprite>("sprite1")->modulate.g, 1.0, 0.4, 0.25, TRANS_QUAD);
	getChild<Tween>("tweenB")->propInterpolate(getChild<Sprite>("sprite1")->modulate.b, 1.0, 0.4, 0.25, TRANS_QUAD);
}