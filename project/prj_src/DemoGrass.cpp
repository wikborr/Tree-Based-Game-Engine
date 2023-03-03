#include "DemoGrass.h"

#include "leaves/leaves2D/Camera2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/leaves2D/ColorRect.h"
#include "leaves/Timer.h"
#include "leaves/Tween.h"
#include "leaves/leaves2D/Area2D.h"
#include "leaves/leaves2D/KinematicBody2D.h"
#include "leaves/leaves2D/ParticleEmitter.h"
#include "leaves/BGLayer.h"
#include "leaves/UILayer.h"
#include "LeafTree.h"

void DemoGrass::ready(){
	std::cout<<"DemoGrass loaded!"<<std::endl;
}

void DemoGrass::inputEvent(){
	if(Input::ins().isKeyJustPressed(GLFW_KEY_F)){
		Settings::ins().goFullscreen();
	}
	if(Input::ins().isKeyJustPressed(GLFW_KEY_ESCAPE)){
		Settings::ins().closeGame();
	}
	if(Input::ins().isKeyJustPressed(GLFW_KEY_ESCAPE)){
		Settings::ins().closeGame();
	}

	if(Input::ins().isKeyJustPressed(GLFW_KEY_TAB)){
		getChild<Tween>("controlsTween1")->reset(true);
		getChild<Tween>("controlsTween2")->reset(true);
		if(getChild<UILayer>("uiLayer")->getChild<Sprite>("controlSprite")->modulate.a > 0.0){
			getChild<Tween>("controlsTween1")->propInterpolate(getChild<UILayer>("uiLayer")->getChild<Sprite>("controlSprite")->modulate.a, 0.3, 0.0, 0.1, TRANS_LIN);
			getChild<Tween>("controlsTween2")->propInterpolate(getChild<UILayer>("uiLayer")->getChild<ColorRect>("controls")->modulate.a, 0.3, 0.0, 0.1, TRANS_LIN);
		}
		else{
			getChild<Tween>("controlsTween1")->propInterpolate(getChild<UILayer>("uiLayer")->getChild<Sprite>("controlSprite")->modulate.a, 0.0, 0.3, 0.1, TRANS_LIN);
			getChild<Tween>("controlsTween2")->propInterpolate(getChild<UILayer>("uiLayer")->getChild<ColorRect>("controls")->modulate.a, 0.0, 0.3, 0.1, TRANS_LIN);
		}
	}
	if(Input::ins().isKeyJustPressed(GLFW_KEY_R)){
		this->leafTree->reloadGrass();
	}
}

void DemoGrass::update(double delta){
	//bg animation
	if(bgAnimFrames != 0){
		if(bgAnimFrames > 59){
			bgAnimFrames = 1;
		}

		if(bgAnimFrames > 39){
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background1")->visible = false;
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background2")->visible = false;
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background3")->visible = true;
		}
		else if(bgAnimFrames > 19){
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background1")->visible = false;
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background2")->visible = true;
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background3")->visible = false;
		}
		else if(bgAnimFrames > 0){
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background1")->visible = true;
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background2")->visible = false;
			getChild<BGLayer>("bgLayer1")->getChild<Sprite>("background3")->visible = false;
		}
		bgAnimFrames +=1;
	}

	//std::cout<<"FPS: "<<static_cast<int>(1/delta)<<std::endl;
}