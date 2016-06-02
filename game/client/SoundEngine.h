#pragma once

#include "fmod.hpp"
#include <iostream>

#include "Game\Player.h"

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();

	void initialize();
	void initializePlayer(Player* player);

	void updateSystem();

	void playLoadingMusic();
	void playMenuMusic();
	void playGameMusic();
	void playEndMusic();

	void mangoShot(float x, float y, float z);
	void jump(int playerId);

	void pauseChannel(FMOD::Channel* channel) {
		if (channel) {
			channel->setPaused(true);
		}
	}
	
private:
	FMOD::System* system_;
	FMOD_RESULT result_;

	FMOD::Sound* loadingMusic_;
	FMOD::Sound* gameMusic_;
	FMOD::Sound* menuMusic_;
	FMOD::Sound* endMusic_;

	FMOD::Sound* mangoShotSound_;
	FMOD::Sound* jumpSound_;
	FMOD::Sound* propellerSound_;

	FMOD::Channel* loadingChannel_ = 0;
	FMOD::Channel* menuChannel_ = 0;
	FMOD::Channel* gameChannel_ = 0;
	FMOD::Channel* endChannel_ = 0;

	Player* player_ = nullptr;

	unsigned int version;
	void* extradriverdata = 0;
};

