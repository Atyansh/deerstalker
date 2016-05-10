#include "Globals.h"

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
Camera Globals::camera = Camera();

LightShader* Globals::lightShader;

Skybox Globals::skybox = Skybox();

boost::asio::io_service Globals::io_service;

tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<protos::Message> Globals::messageQueue = std::deque<protos::Message>();

char Globals::currentHeader[util::HEADER_SIZE] = "";

uint32_t Globals::ID = 0;