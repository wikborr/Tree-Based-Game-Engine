#include "Enemy.h"

#include "leaves/leaves2D/Camera2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/Timer.h"
#include "leaves/Tween.h"
#include "leaves/leaves2D/Area2D.h"
#include "leaves/leaves2D/KinematicBody2D.h"
#include "leaves/leaves2D/ParticleEmitter.h"
#include "MainCharacter.h"

void Enemy::ready(){
}

void Enemy::update(double delta){
	if(!firstUpdateComplete){
		firstUpdateComplete = true;
		return;
	}
	if(getChild<Tween>("tweenG2")->completed()){
		if(getChild<Sprite>("sprite1")->modulate.g < 1.0){
			getChild<Tween>("tweenG2")->reset(true);
			getChild<Tween>("tweenB2")->reset(true);
			getChild<Tween>("tweenG2")->propInterpolate(getChild<Sprite>("sprite1")->modulate.g, 0.0, 1.0, 0.25, TRANS_QUAD);
			getChild<Tween>("tweenB2")->propInterpolate(getChild<Sprite>("sprite1")->modulate.b, 0.0, 1.0, 0.25, TRANS_QUAD);
		}
	}

	if(this->bodyEntered(getParent()->getChild<KinematicBody2D>("mainCharacter"))){
		if(activated == 0 && scale.x > 1){
			activated = 1;
			getParent()->getChild<MainCharacter>("mainCharacter")->collision();
			getChild<Tween>("tweenG2")->propInterpolate(getChild<Sprite>("sprite1")->modulate.g, 1.0, 0.0, 0.25, TRANS_QUAD);
			getChild<Tween>("tweenB2")->propInterpolate(getChild<Sprite>("sprite1")->modulate.b, 1.0, 0.0, 0.25, TRANS_QUAD);
		}
		else if(activated == 1 || (activated == 0 && scale.x <= 1)){
			activated = 2;
			static_cast<Timer*>(getChild("timer1"))->start();
			static_cast<Timer*>(getChild("timer2"))->start();
			getChild<Tween>("tween1")->propInterpolate(getChild<Sprite>("sprite1")->modulate.a, 1.0, 0.0, 1.0, TRANS_LIN);
			getChild<Tween>("tweenG")->propInterpolate(getChild<Sprite>("sprite1")->modulate.g, 1.0, 0.0, 0.25, TRANS_QUAD);
			getChild<Tween>("tweenB")->propInterpolate(getChild<Sprite>("sprite1")->modulate.b, 1.0, 0.0, 0.25, TRANS_QUAD);
			getParent()->getChild<MainCharacter>("mainCharacter")->collision();
		}
	}

	if(static_cast<Timer*>(getChild("timer1"))->timeout()){
		getChild<ParticleEmitter>("particles1")->emit(glm::vec2(float(rand()%201)/100  - 1.f, float(rand()%201)/100  - 1.f)*float(1000*delta));
	}
	if(static_cast<Timer*>(getChild("timer2"))->timeout()){
		getChild<Timer>("timer1")->stop();
		this->visible = false;
	}
}