#include "leaves/Timer.h"

Timer::Timer(Leaf* parent){
	this->name = "Timer";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"oneShot", {"bool", &(this->oneShot)}},
		{"paused", {"bool", &(this->paused)}},
		{"waitTime", {"double", &(this->waitTime)}},
	};
	this->propPointers.insert(props.begin(), props.end());

	this->lastTime = glfwGetTime();
}

std::string Timer::leafOperation(){
	this->timeoutSignal = false;

	double newTime = glfwGetTime();
	double timeDelta = newTime - this->lastTime;
	this->lastTime = newTime;

	if(this->timeLeft!=-1.0 && !this->paused){
		this->timeLeft -= timeDelta;
		if(this->timeLeft < 0.0){
			if(this->oneShot) this->timeLeft = -1.0;
			else this->timeLeft = this->waitTime;
			this->timeoutSignal = true;
		}
	}

	return "";
}

bool Timer::isStopped() const{
	if(this->waitTime == -1.0){
		return true;
	}
	return false;
}

void Timer::start(double time){
	if(time==-1.0){
		this->timeLeft = this->waitTime;
	}
	else{
		this->timeLeft = time;
	}
}

void Timer::stop(){
	this->waitTime = -1.0;
}

bool Timer::timeout(){
	return this->timeoutSignal;
}