#include "SoundEngine.h"
#include "Globals.h"

SoundEngine::SoundEngine() {
}


SoundEngine::~SoundEngine() {
}

void SoundEngine::initialize() {
	int numsubsounds;

	result_ = FMOD::System_Create(&system_);
	result_ = system_->getVersion(&version);
	result_ = system_->init(100, FMOD_INIT_NORMAL, extradriverdata);

	result_ = system_->createStream("Sounds\\loading_music.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &loadingMusic_);
	result_ = system_->createStream("Sounds\\menu_music.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &menuMusic_);
	result_ = system_->createStream("Sounds\\Spazzkid.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &gameMusic_);
	result_ = system_->createStream("Sounds\\end_music.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &endMusic_);

	result_ = system_->createSound("Sounds\\mango_shooting.wav", FMOD_3D, 0, &mangoShotSound_);
	mangoShotSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\jump.wav", FMOD_3D, 0, &jumpSound_);
	jumpSound_->set3DMinMaxDistance(5, 5000);
}

void SoundEngine::initializePlayer(Player* player) {
	player_ = player;
}

void SoundEngine::updateSystem() {
	if (player_) {
		glm::vec3 pos = player_->getPosition();
		const FMOD_VECTOR position = { pos.x, pos.y, pos.z };
		system_->set3DListenerAttributes(0, &position, NULL, NULL, NULL);
	}
	system_->update();
}

void SoundEngine::playLoadingMusic() {
	pauseChannel(menuChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(endChannel_);
	result_ = system_->playSound(loadingMusic_, 0, false, &loadingChannel_);
}

void SoundEngine::playMenuMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(endChannel_);
	result_ = system_->playSound(menuMusic_, 0, false, &menuChannel_);
}

void SoundEngine::playGameMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(endChannel_);
	result_ = system_->playSound(gameMusic_, 0, false, &gameChannel_);
}

void SoundEngine::playEndMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(gameChannel_);
	result_ = system_->playSound(endMusic_, 0, false, &endChannel_);
}

void SoundEngine::mangoShot(float x, float y, float z) {
	FMOD::Channel* channel = nullptr;
	FMOD_VECTOR position = { x, y, z };
	result_ = system_->playSound(mangoShotSound_, 0, true, &channel);
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}

void SoundEngine::jump(int playerId) {
	Player* player = dynamic_cast<Player*>(Globals::gameObjects.playerMap[playerId]);
	FMOD::Channel* channel = nullptr;
	glm::vec3 pos = player->getPosition();
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };
	result_ = system_->playSound(jumpSound_, 0, true, &channel);
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}