#include "leaves/leaves2D/KinematicBody2D.h"
#include "servers/Physics2D.h"

KinematicBody2D::KinematicBody2D(Leaf* parent){
	this->name = "KinematicBody2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
	};
	this->propPointers.insert(props.begin(), props.end());
}

BodyType KinematicBody2D::bodyType(){
	return KINEMATIC_BODY;
}

std::string KinematicBody2D::leafOperation(){
	PhysicsBody2D::leafOperation();
	return "";
}

std::vector<CollisionEvent> KinematicBody2D::getCollisionEvents(){
	return this->collisionEvents;
}

glm::vec2 KinematicBody2D::moveAndCollide(glm::vec2 velocity){
	velocity = glm::round(velocity);
	this->collisionEvents.clear();
	glm::vec2 oldPos = this->position;
	this->position += velocity;
	Physics2D::ins().updateShapePositions(this);
	glm::vec2 newPos = this->globalPosition;

	this->collisionEvents = Physics2D::ins().getCollisionEvents(this, velocity);

	glm::vec2 vecSign = glm::vec2(1.f);
	if(velocity.x < 0.f) vecSign.x = -1.f;
	else if(velocity.x == 0.f) vecSign.x = this->lastVecSign.x;
	if(velocity.y < 0.f) vecSign.y = -1.f;
	else if(velocity.y == 0.f) vecSign.y = this->lastVecSign.y;
	this->lastVecSign = vecSign;

	glm::vec2 finalOverlap = glm::vec2(0.f);
	for(const auto& i: this->collisionEvents){
		if(finalOverlap != glm::vec2(0.f)){
			if(abs(i.overlapVector.x) > abs(finalOverlap.x)) finalOverlap.x = i.overlapVector.x;
			if(abs(i.overlapVector.y) > abs(finalOverlap.y)) finalOverlap.y = i.overlapVector.y;
		}
		else{
			finalOverlap = i.overlapVector;
		}
	}
	finalOverlap = glm::round(finalOverlap);
	finalOverlap *= vecSign;
	this->lastOverlapVector = finalOverlap;
	glm::vec2 actualVelocity = velocity - finalOverlap;
	this->position = oldPos;
	Physics2D::ins().updateShapePositions(this);
	this->position += actualVelocity;
	return actualVelocity;
}

bool KinematicBody2D::isOnFloor(){
	if(this->lastOverlapVector.y < 0){
		return true;
	}
	return false;
}

bool KinematicBody2D::isOnCeiling(){
	if(this->lastOverlapVector.y > 0){
		return true;
	}
	return false;
}

bool KinematicBody2D::isOnWall(){
	if(this->lastOverlapVector.x  != 0){
		return true;
	}
	return false;
}