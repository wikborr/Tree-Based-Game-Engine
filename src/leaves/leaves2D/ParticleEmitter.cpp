#include "leaves/leaves2D/ParticleEmitter.h"
#include "servers/ResourceManager.h"

ParticleEmitter::ParticleEmitter(Leaf* parent){
	this->name = "ParticleEmitter";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"texture", {"texture", &(this->texture)}},
	};
	this->propPointers.insert(props.begin(), props.end());
	this->lastTime = glfwGetTime();
}

ParticleEmitter::~ParticleEmitter(){
	ResourceManager::ins().unloadTexture(this->texture);
	std::vector<Particle*> oldParticles = this->activeParticles;
	for(auto i: oldParticles){
		ResourceManager::ins().unloadParticle(i);
	}
}

std::string ParticleEmitter::leafOperation(){
	Leaf2D::leafOperation();
	double newTime = glfwGetTime();
	double timeDelta = newTime - this->lastTime;
	this->lastTime = newTime;

	if(this->particlesLeaf == nullptr){
		Leaf* nextLeaf = this;
		while(nextLeaf->parent != nullptr) nextLeaf = nextLeaf->parent;
		this->particlesLeaf = nextLeaf->addChild<Leaf2D>("_"+this->name+"_particlesLeaf");
	}

	std::vector<Particle*> newParticles;
	for(auto& i: this->activeParticles){
		i->timeLeft -= timeDelta;
		i->spriteLeaf->position += i->velocity;
		if(i->timeLeft <= 0.f){
			ResourceManager::ins().unloadParticle(i);
		}
		else{
			newParticles.push_back(i);
		}
	}
	this->activeParticles = newParticles;
	return "";
}

void ParticleEmitter::emit(glm::vec2 velocity, glm::vec2 offset, float lifetime){
	Particle* particle;
	if(lifetime != -1.f) particle = ResourceManager::ins().loadParticle(this->texture, velocity, this->globalPosition+offset, this->scale,  lifetime);
	else particle = ResourceManager::ins().loadParticle(this->texture, velocity, this->globalPosition+offset, this->scale, this->lifetime);
	this->particlesLeaf->addChild<Sprite>(particle->spriteLeaf);
	this->activeParticles.push_back(particle);
}