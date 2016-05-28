#include "Globals.h"

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
Camera_Original Globals::camera = Camera_Original();
Camera Globals::cam = Camera();
GameObjects Globals::gameObjects = GameObjects();
boost::asio::io_service Globals::io_service;
tcp::socket Globals::socket = tcp::socket(Globals::io_service);
char Globals::currentHeader[util::HEADER_SIZE] = "";
uint32_t Globals::ID = 0;
bool Globals::socketError = false;
unordered_map<HatType, Models> Globals::hatModelsMap;
SoundEngine Globals::soundEngine;