#pragma once

#include "Leaf.h"

//leaf for playing streams of sound
class AudioPlayer: public Leaf{
public:
	AudioPlayer(Leaf* parent = nullptr);
	virtual ~AudioPlayer();
	virtual std::string leafOperation() override;

	float volume = 0.f;
	float pitchScale = 1.f;
	bool streamPaused = false;
	bool playing = false;
	//audioStream
	//audioBus
};