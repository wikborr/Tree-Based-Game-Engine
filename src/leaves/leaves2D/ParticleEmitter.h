#pragma once

#include "leaves/Leaf2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "resources/Texture.h"
#include "resources/Particle.h"

//leaf for generating large amounts of textures
class ParticleEmitter: public Leaf2D{
private:
	double lastTime = 0.0;
	Leaf2D* particlesLeaf = nullptr;
	std::vector<Particle*> activeParticles;
public:
	ParticleEmitter(Leaf* parent = nullptr);
	virtual ~ParticleEmitter();
	virtual std::string leafOperation() override;

	float lifetime = 1.f;
	Texture* texture = nullptr;

	void emit(glm::vec2 velocity, glm::vec2 offset = glm::vec2(0.f), float lifetime = -1.f);
};