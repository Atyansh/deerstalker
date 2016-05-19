#include "Globals.h"

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
Camera_Original Globals::camera = Camera_Original();
Camera Globals::cam = Camera();

boost::asio::io_service Globals::io_service;

tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<protos::Message> Globals::messageQueue = std::deque<protos::Message>();
std::mutex Globals::queueLock;
char Globals::currentHeader[util::HEADER_SIZE] = "";

uint32_t Globals::ID = 0;

bool Globals::shoot = false;