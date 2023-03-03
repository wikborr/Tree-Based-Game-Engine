#include "leaves/AudioPlayer.h"
#include "servers/ResourceManager.h"

AudioPlayer::AudioPlayer(Leaf* parent){
	this->name = "AudioPlayer";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"volume", {"float", &(this->volume)}},
		{"pitchScale", {"float", &(this->pitchScale)}},
		{"streamPaused", {"bool", &(this->streamPaused)}},
		{"playing", {"bool", &(this->playing)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

AudioPlayer::~AudioPlayer(){

}

std::string AudioPlayer::leafOperation(){
	Leaf::leafOperation();
	return "";
}
