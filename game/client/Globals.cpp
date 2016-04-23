#include "Globals.h"

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
Camera Globals::camera = Camera();

boost::asio::io_service Globals::io_service = boost::asio::io_service();
tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<protos::Message> Globals::messageQueue = std::deque<protos::Message>();

char Globals::currentHeader[4] = "";

uint32_t Globals::ID = 0;