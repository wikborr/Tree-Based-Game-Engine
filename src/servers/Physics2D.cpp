#include "servers/Physics2D.h"
#include "leaves/leaves2D/CollisionShape2D.h"

Physics2D& Physics2D::ins(){
	static Physics2D instance;
	return instance;
}

std::string Physics2D::init(LeafTree* leafTree){
	this->leafTree = leafTree;
	return "";
}

std::vector<PhysicsBody2D*> Physics2D::getCollidingBodies(CollisionObject2D* collisionObject){
	std::vector<CollisionEvent> collisionEvents = this->getCollisionEvents(collisionObject);
	std::vector<PhysicsBody2D*> bodies;
	for(const auto& i: collisionEvents){
		bodies.push_back(static_cast<PhysicsBody2D*>(i.body));
	}
	return bodies;
}

std::vector<CollisionEvent> Physics2D::getCollisionEvents(CollisionObject2D* collisionObject, glm::vec2 velocity){
	glm::vec2 absVel = glm::normalize(glm::abs(velocity));
	std::vector<PhysicsBody2D*> allBodies;
	//get all active bodies
	std::vector<Leaf*> leaves = this->leafTree->getAllLeaves();
	for(const auto &i: leaves){
		if(i->propPointers.count("bodyActive") > 0 && static_cast<PhysicsBody2D*>(i)->bodyActive && static_cast<CollisionObject2D*>(i) != collisionObject && static_cast<CollisionObject2D*>(i)->collisionLayer <= collisionObject->collisionMask)
			allBodies.push_back(static_cast<PhysicsBody2D*>(i));
	}
	//broad collision detection - can be improved with AABB tree
	glm::vec4 objRect = this->getObjectBroadSize(collisionObject); //minX, minY, maxX, maxY
	std::vector<PhysicsBody2D*> broadCollidingBodies;
	for(const auto& i: allBodies){
		glm::vec4 bodyRect = this->getObjectBroadSize(i);
		if(bodyRect.x < objRect.z && objRect.x < bodyRect.z && bodyRect.y < objRect.w && objRect.y < bodyRect.w){
			broadCollidingBodies.push_back(i);
		}
	}
	//narrow collision detection
	std::vector<CollisionEvent> collisionEvents;
	for(const auto& i: broadCollidingBodies){
		glm::vec2 finalOverlap = glm::vec2(0.f);
		for(const auto& j: i->children){
			if(j->propPointers.count("oneWayCollision") > 0 && !static_cast<CollisionShape2D*>(j)->disabled && static_cast<CollisionShape2D*>(j)->shape != nullptr){
				for(const auto& k: collisionObject->children){
					if(k->propPointers.count("oneWayCollision") > 0 && !static_cast<CollisionShape2D*>(k)->disabled && static_cast<CollisionShape2D*>(k)->shape != nullptr){
						CollisionShape2D* shape1 = static_cast<CollisionShape2D*>(k);
						CollisionShape2D* shape2 = static_cast<CollisionShape2D*>(j);
						glm::vec2 overlap = shape1->shape->collidesWith(shape1->globalTransform, shape2->shape->toPoints(shape2->globalTransform), velocity);
						if(overlap != glm::vec2(0.f)){
							if(finalOverlap != glm::vec2(0.f)){
								if(abs(overlap.x) > abs(finalOverlap.x)) finalOverlap.x = overlap.x;
								if(abs(overlap.y) > abs(finalOverlap.y)) finalOverlap.y = overlap.y;
							}
							else{
								finalOverlap = overlap;
							}
						}
					}
				}
			}
		}
		if(finalOverlap != glm::vec2(0.f)){
			CollisionEvent colEvent;
			colEvent.body = i;
			colEvent.overlapVector = finalOverlap;
			collisionEvents.push_back(colEvent);
		}
	}
	return collisionEvents;
}

glm::vec4 Physics2D::getObjectBroadSize(CollisionObject2D* collisionObject){
	glm::vec4 objRect = glm::vec4(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX); //minX, minY, maxX, maxY
	for(const auto &i: collisionObject->children){
		if(i->propPointers.count("oneWayCollision") > 0 && !static_cast<CollisionShape2D*>(i)->disabled && static_cast<CollisionShape2D*>(i)->shape != nullptr){
			glm::vec2 shapeBroadSize = static_cast<CollisionShape2D*>(i)->shape->broadSize(static_cast<CollisionShape2D*>(i)->globalTransform);
			glm::vec4 shapeRect = glm::vec4(
				static_cast<CollisionShape2D*>(i)->globalPosition.x - shapeBroadSize.x/2, 
				static_cast<CollisionShape2D*>(i)->globalPosition.y - shapeBroadSize.y/2,
				static_cast<CollisionShape2D*>(i)->globalPosition.x + shapeBroadSize.x/2, 
				static_cast<CollisionShape2D*>(i)->globalPosition.y + shapeBroadSize.y/2
			);
			if(shapeRect.x < objRect.x) objRect.x = shapeRect.x;
			if(shapeRect.y < objRect.y) objRect.y = shapeRect.y;
			if(shapeRect.z > objRect.z) objRect.z = shapeRect.z;
			if(shapeRect.w > objRect.w) objRect.w = shapeRect.w;
		}
	}
	return objRect;
}

void Physics2D::updateShapePositions(CollisionObject2D* collisionObject){
	collisionObject->Leaf2D::leafOperation();
	for(const auto& i: collisionObject->children){
		if(i->propPointers.count("oneWayCollision") > 0){
			static_cast<CollisionShape2D*>(i)->Leaf2D::leafOperation();
		}
	}
}