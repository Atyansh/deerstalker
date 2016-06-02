#pragma once

#include "fmod.hpp"
#include <iostream>

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();

	void initialize();

	void updateSystem() {
		system_->update();
	}

	void playLoadingMusic();
	void playMenuMusic();
	void playGameMusic();
	void playEndMusic();

	void pauseChannel(FMOD::Channel* channel) {
		if (channel) {
			channel->setPaused(true);
		}
	}
	
private:
	FMOD::System* system_;

	FMOD::Sound* loadingMusic_;
	FMOD::Sound* gameMusic_;
	FMOD::Sound* menuMusic_;
	FMOD::Sound* endMusic_;

	FMOD::Sound* mangoShotSound;
	FMOD::Sound* propellerSound;

	FMOD::Channel* loadingChannel_ = 0;
	FMOD::Channel* menuChannel_ = 0;
	FMOD::Channel* gameChannel_ = 0;
	FMOD::Channel* endChannel_ = 0;

	unsigned int version;
	void* extradriverdata = 0;
};

