#include "SoundEngine.h"

SoundEngine::SoundEngine() {
}


SoundEngine::~SoundEngine() {
}

void SoundEngine::initialize() {
	int numsubsounds;

	FMOD_RESULT result;

	result = FMOD::System_Create(&system_);
	result = system_->getVersion(&version);
	result = system_->init(100, FMOD_INIT_NORMAL, extradriverdata);

	result = system_->createStream("Sounds\\loading_music.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &loadingMusic_);
	result = system_->createStream("Sounds\\menu_music.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &menuMusic_);
	result = system_->createStream("Sounds\\Spazzkid.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &gameMusic_);
	result = system_->createStream("Sounds\\end_music.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &endMusic_);

	result = system_->createSound("Sounds\\mango_shooting.wav", FMOD_DEFAULT, 0, &sound2);

}

void SoundEngine::playLoadingMusic() {
	pauseChannel(menuChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(endChannel_);
	FMOD_RESULT result = system_->playSound(loadingMusic_, 0, false, &loadingChannel_);
}

void SoundEngine::playMenuMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(endChannel_);
	FMOD_RESULT result = system_->playSound(menuMusic_, 0, false, &menuChannel_);
}

void SoundEngine::playGameMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(endChannel_);
	FMOD_RESULT result = system_->playSound(gameMusic_, 0, false, &gameChannel_);
}

void SoundEngine::playEndMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(gameChannel_);
	FMOD_RESULT result = system_->playSound(endMusic_, 0, false, &endChannel_);
}