#pragma once

#include "Leaf.h"

//leaf responsible for timed events
class Timer: public Leaf{
protected:
	double lastTime = 0.0;
	bool timeoutSignal = false;
public:
	Timer(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	bool oneShot = true; //doesn't loop
	bool paused = false;
	double waitTime = 1.0;
	double timeLeft = -1.0;

	bool isStopped() const;
	void start(double time = -1.0);
	void stop();

	//signal methodss
	bool timeout();
};