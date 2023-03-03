#include "leaves/Tween.h"

Tween::Tween(Leaf* parent){
	this->name = "Tween";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"playSpeed", {"double", &(this->playSpeed)}},
		{"oneShot", {"bool", &(this->oneShot)}},
		{"paused", {"bool", &(this->paused)}},
	};
	this->propPointers.insert(props.begin(), props.end());

	this->lastTime = glfwGetTime();
}

std::string Tween::leafOperation(){
	this->completedSignal = false;

	double newTime = glfwGetTime();
	double timeDelta = (newTime - this->lastTime)*this->playSpeed;
	this->lastTime = newTime;

	if(this->currentTime >= 0.0 && !this->paused && this->property != nullptr){
		this->currentTime += timeDelta;

		if(this->currentTime >= this->duration){
			if(this->oneShot){
				this->currentTime = -1.0;
			}
			else{
				this->currentTime = 0.0;
			}
			this->completedSignal = true;
			if(this->floatType == TYPE_FLOAT){
				float& propertyRef = *static_cast<float*>(this->property);
				propertyRef = static_cast<float>(this->finValue);
			}
			else if(this->floatType == TYPE_DOUBLE){
				double& propertyRef = *static_cast<double*>(this->property);
				propertyRef = this->finValue;
			}
		}
		else{
			double currentValue = 0.0;
			if(this->transType == TRANS_LIN){
				currentValue = ((this->finValue - this->initValue)*this->currentTime)/this->duration + this->initValue;
			}
			else if(this->transType == TRANS_QUAD){
				if(this->easeType == EASE_IN_OUT){
					double midTime = this->duration / 2.0;
					double midValue = (this->initValue + this->finValue) / 2.0;

					if(this->currentTime < midTime){
						currentValue = (midValue - this->initValue)/pow(midTime, 2)*pow(this->currentTime, 2) + this->initValue;
					}
					else{
						currentValue = (midValue - this->finValue)/pow(midTime, 2)*pow(this->currentTime - this->duration, 2) + this->finValue;
					}
				}
				else if(this->easeType == EASE_OUT_IN){
					double midTime = this->duration / 2.0;
					double midValue = (this->initValue + this->finValue) / 2.0;

					if(this->currentTime < midTime){
						currentValue = (this->initValue - midValue)/pow(midTime, 2)*pow(this->currentTime - midTime, 2) + midValue;
					}
					else{
						currentValue = (this->finValue - midValue)/pow(midTime, 2)*pow(this->currentTime - midTime, 2) + midValue;
					}
				}
				else if(this->easeType == EASE_IN){
					currentValue = (this->finValue - this->initValue)/pow(this->duration, 2)*pow(this->currentTime, 2) + this->initValue;
				}
				else if(this->easeType == EASE_OUT){
					currentValue = (this->initValue - this->finValue)/pow(this->duration, 2)*pow(this->currentTime - this->duration, 2) + this->finValue;
				}
			}

			if(this->floatType == TYPE_FLOAT){
				float& propertyRef = *static_cast<float*>(this->property);
				propertyRef = static_cast<float>(currentValue);
			}
			else if(this->floatType == TYPE_DOUBLE){
				double& propertyRef = *static_cast<double*>(this->property);
				propertyRef = currentValue;
			}
		}
	}

	return "";
}

void Tween::stop(){
	this->paused = true;
}

void Tween::resume(){
	this->paused = false;
}

void Tween::reset(bool withoutProperty){
	this->paused = false;
	this->currentTime = -1.0;
	if(!withoutProperty){
		if(this->floatType == TYPE_FLOAT){
			float& propertyRef = *static_cast<float*>(this->property);
			propertyRef = static_cast<float>(this->initValue);
		}
		else if(this->floatType == TYPE_DOUBLE){
			double& propertyRef = *static_cast<double*>(this->property);
			propertyRef = this->initValue;
		}
	}
}

bool Tween::isActive() const{
	if(this->currentTime >= 0.0) return true;
	return false;
}

void Tween::propInterpolate(float& property, double initValue, double finValue, double duration, TransType trans, EaseType ease){
	this->floatType = TYPE_FLOAT;
	this->property = static_cast<void*>(&property);
	this->propInterpolateReal(initValue, finValue, duration, trans, ease);
}
void Tween::propInterpolate(double& property, double initValue, double finValue, double duration, TransType trans, EaseType ease){
	this->floatType = TYPE_DOUBLE;
	this->property = static_cast<void*>(&property);
	this->propInterpolateReal(initValue, finValue, duration, trans, ease);
}

void Tween::propInterpolateReal(double initValue, double finValue, double duration, TransType trans, EaseType ease){
	if(duration != 0.0){
		this->duration = duration;
	}
	else{
		this->duration = 1.0;
	}
	this->paused = false;
	this->initValue = initValue;
	this->finValue = finValue;
	this->transType = trans;
	this->easeType = ease;
	this->currentTime = 0.0;
}

bool Tween::completed(){
	return this->completedSignal;
}