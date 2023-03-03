#include "resources/Particle.h"

Particle::Particle(){
	this->spriteLeaf = new Sprite;
}

Particle::~Particle(){
	delete this->spriteLeaf;
}