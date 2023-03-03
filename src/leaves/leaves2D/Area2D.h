#pragma once

#include "leaves/leaves2D/CollisionObject2D.h"
#include "leaves/leaves2D/PhysicsBody2D.h"

//leaf for collision interaction
class Area2D: public CollisionObject2D{
private:
	std::vector<PhysicsBody2D*> bodyEnteredSignal;
	std::vector<PhysicsBody2D*> bodyExitedSignal;

	std::vector<PhysicsBody2D*> overlappingBodies;
public:
	Area2D(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	bool monitoring = true;

	std::vector<PhysicsBody2D*> getOverlappingBodies() const;
	bool overlapsBody(PhysicsBody2D* body);

	//signal methods
	bool bodyEntered(PhysicsBody2D* body);
	bool bodyExited(PhysicsBody2D* body);
};