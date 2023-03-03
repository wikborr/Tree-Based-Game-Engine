#pragma once

#include "Leaf.h"

enum TransType{
	TRANS_LIN,
	TRANS_QUAD
};

enum EaseType{
	EASE_IN,
	EASE_OUT,
	EASE_IN_OUT,
	EASE_OUT_IN
};

enum FloatType{
	TYPE_FLOAT,
	TYPE_DOUBLE
};

//leaf responsible for transitioning properties between two values
class Tween: public Leaf{
protected:
	void propInterpolateReal(double initValue, double finValue, double duration, TransType trans = TRANS_LIN, EaseType ease = EASE_IN_OUT);

	double initValue = 0.0;
	double finValue = 0.0;
	double duration = 1.0;
	TransType transType = TRANS_LIN;
	EaseType easeType = EASE_IN_OUT;
	FloatType floatType = TYPE_FLOAT;
	void* property = nullptr;

	double lastTime = 0.0;
	bool completedSignal = false;

public:
	Tween(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	double playSpeed = 1.0;
	bool oneShot = true; //doesn't loop
	bool paused = false;
	double currentTime = -1.0;

	void stop();
	void resume();
	void reset(bool withoutProperty = false);
	bool isActive() const;

	void propInterpolate(float& property, double initValue, double finValue, double duration, TransType trans = TRANS_LIN, EaseType ease = EASE_IN_OUT);
	void propInterpolate(double& property, double initValue, double finValue, double duration, TransType trans = TRANS_LIN, EaseType ease = EASE_IN_OUT);

	//signal methods
	bool completed();
};