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
	result_ = system_->createStream("Sounds\\gravity.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &gravityMusic_);

	result_ = system_->createSound("Sounds\\mango_shooting.wav", FMOD_3D, 0, &mangoShotSound_);
	mangoShotSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\jump.wav", FMOD_3D, 0, &jumpSound_);
	jumpSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\hat_spawn.wav", FMOD_3D, 0, &hatSpawnSound_);
	hatSpawnSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\wear_hat.wav", FMOD_3D, 0, &wearHatSound_);
	wearHatSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\wuson.wav", FMOD_3D, 0, &wusonSound_);
	wusonSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\propeller.mp3", FMOD_3D, 0, &propellerSound_);
	propellerSound_->set3DMinMaxDistance(5, 5000);

	result_ = system_->createSound("Sounds\\punch.wav", FMOD_3D, 0, &punchSound_);
	punchSound_->set3DMinMaxDistance(5, 5000);
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
	pauseChannel(gravityChannel_);

	if (loadingChannel_) {
		bool paused;
		loadingChannel_->getPaused(&paused);
		if (paused) {
			loadingChannel_->setPaused(false);
		}
		return;
	}

	result_ = system_->playSound(loadingMusic_, 0, false, &loadingChannel_);
}

void SoundEngine::playMenuMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(endChannel_);
	pauseChannel(gravityChannel_);

	if (menuChannel_) {
		bool paused;
		menuChannel_->getPaused(&paused);
		if (paused) {
			menuChannel_->setPaused(false);
		}
		return;
	}

	result_ = system_->playSound(menuMusic_, 0, false, &menuChannel_);
}

void SoundEngine::playGameMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(endChannel_);
	pauseChannel(gravityChannel_);

	if (gameChannel_) {
		bool paused;
		gameChannel_->getPaused(&paused);
		if (paused) {
			gameChannel_->setPaused(false);
		}
		return;
	}

	result_ = system_->playSound(gameMusic_, 0, false, &gameChannel_);
}

void SoundEngine::playEndMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(gravityChannel_);

	if (endChannel_) {
		bool paused;
		endChannel_->getPaused(&paused);
		if (paused) {
			endChannel_->setPaused(false);
		}
		return;
	}

	result_ = system_->playSound(endMusic_, 0, false, &endChannel_);
}

void SoundEngine::playGravityMusic() {
	pauseChannel(loadingChannel_);
	pauseChannel(menuChannel_);
	pauseChannel(gameChannel_);
	pauseChannel(endChannel_);

	if (gravityChannel_) {
		bool paused;
		gravityChannel_->getPaused(&paused);
		if (paused) {
			gravityChannel_->setPaused(false);
		}
		return;
	}

	result_ = system_->playSound(gravityMusic_, 0, false, &gravityChannel_);
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

void SoundEngine::hatSpawn(float x, float y, float z) {
	FMOD::Channel* channel = nullptr;
	FMOD_VECTOR position = { x, y, z };
	result_ = system_->playSound(hatSpawnSound_, 0, true, &channel);
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}

void SoundEngine::wearHat(int playerId) {
	Player* player = dynamic_cast<Player*>(Globals::gameObjects.playerMap[playerId]);
	FMOD::Channel* channel = nullptr;
	glm::vec3 pos = player->getPosition();
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };
	result_ = system_->playSound(wearHatSound_, 0, true, &channel);
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}

void SoundEngine::wuson(int playerId) {
	Player* player = dynamic_cast<Player*>(Globals::gameObjects.playerMap[playerId]);
	FMOD::Channel* channel = wusonChannels[playerId];

	glm::vec3 pos = player->getPosition();
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };

	if (channel) {
		bool playing = false;
		if (channel->isPlaying(&playing));
		if (playing) {
			channel->set3DAttributes(&position, 0);
			return;
		}
	}

	result_ = system_->playSound(wusonSound_, 0, true, &channel);
	wusonChannels[playerId] = channel;
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}

void SoundEngine::propeller(int playerId) {
	Player* player = dynamic_cast<Player*>(Globals::gameObjects.playerMap[playerId]);
	FMOD::Channel* channel = propellerChannels[playerId];

	glm::vec3 pos = player->getPosition();
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };

	if (channel) {
		bool playing = false;
		if (channel->isPlaying(&playing));
		if (playing) {
			channel->set3DAttributes(&position, 0);
			return;
		}
	}

	result_ = system_->playSound(propellerSound_, 0, true, &channel);
	propellerChannels[playerId] = channel;
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}

void SoundEngine::punch(int playerId) {
	Player* player = dynamic_cast<Player*>(Globals::gameObjects.playerMap[playerId]);
	FMOD::Channel* channel = punchChannels[playerId];

	glm::vec3 pos = player->getPosition();
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };

	if (channel) {
		bool playing = false;
		if (channel->isPlaying(&playing));
		if (playing) {
			channel->set3DAttributes(&position, 0);
			return;
		}
	}

	result_ = system_->playSound(punchSound_, 0, true, &channel);
	punchChannels[playerId] = channel;
	result_ = channel->set3DAttributes(&position, 0);
	result_ = channel->setPaused(false);
}
