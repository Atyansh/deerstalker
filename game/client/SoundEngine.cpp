#include "SoundEngine.h"

SoundEngine::SoundEngine() {
}


SoundEngine::~SoundEngine() {
}

void SoundEngine::initialize() {
	int numsubsounds;

	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	result = system->createStream("Sounds\\Spazzkid.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &gameMusic);
	result = gameMusic->getNumSubSounds(&numsubsounds);


	if (numsubsounds) {
		std::cerr << "SUB SOUND" << std::endl;
		gameMusic->getSubSound(0, &gameMusic);
	}
}

void SoundEngine::playMenuMusic() {

}

void SoundEngine::playGameMusic() {
	result = system->playSound(gameMusic, 0, false, &channel);
}