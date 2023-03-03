#include "leaves/leaves2D/Area2D.h"
#include "servers/Physics2D.h"

Area2D::Area2D(Leaf* parent){
	this->name = "Area2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"monitoring", {"bool", &(this->monitoring)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

std::string Area2D::leafOperation(){
	CollisionObject2D::leafOperation();
	this->bodyEnteredSignal.clear();
	this->bodyExitedSignal.clear();

	if(monitoring){
		std::vector<PhysicsBody2D*> allBodies = Physics2D::ins().getCollidingBodies(this);
		std::vector<PhysicsBody2D*> newOverlap;
		for(auto i: this->overlappingBodies){
			if(std::find(allBodies.begin(), allBodies.end(), i) != allBodies.end()){
				newOverlap.push_back(i);
			}
			else{
				this->bodyExitedSignal.push_back(i);
			}
		}
		this->overlappingBodies = newOverlap;
		for(const auto &i: allBodies){
			if(std::find(this->overlappingBodies.begin(), this->overlappingBodies.end(), i) == this->overlappingBodies.end()){
				this->bodyEnteredSignal.push_back(i);
				this->overlappingBodies.push_back(i);
			}
		}
	}

	return "";
}

std::vector<PhysicsBody2D*> Area2D::getOverlappingBodies() const{
	return this->overlappingBodies;
}

bool Area2D::overlapsBody(PhysicsBody2D* body){
	if(std::find(this->overlappingBodies.begin(), this->overlappingBodies.end(), body) != this->overlappingBodies.end()){
		return true;
	}
	return false;
}

bool Area2D::bodyEntered(PhysicsBody2D* body){
	if(std::find(this->bodyEnteredSignal.begin(), this->bodyEnteredSignal.end(), body) != this->bodyEnteredSignal.end()){
		return true;
	}
	return false;
}

bool Area2D::bodyExited(PhysicsBody2D* body){
	if(std::find(this->bodyExitedSignal.begin(), this->bodyExitedSignal.end(), body) != this->bodyExitedSignal.end()){
		return true;
	}
	return false;
}