#pragma once

#include "fmod.hpp"
#include <iostream>

class SoundEngine {
public:
	SoundEngine();
	~SoundEngine();

	void initialize();

	void playMenuMusic();
	
	void playGameMusic();

private:
	FMOD::System* system;
	FMOD::Sound* gameMusic;
	FMOD::Sound* menuMusic;
	FMOD::Channel* channel = 0;
	FMOD_RESULT result;
	unsigned int version;
	void* extradriverdata = 0;
};

