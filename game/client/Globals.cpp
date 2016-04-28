#include "Globals.h"

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
Camera Globals::camera = Camera();


boost::asio::io_service io_serv;
boost::asio::io_service * Globals::io_service = &io_serv;

tcp::socket Globals::socket = tcp::socket(*Globals::io_service);

std::deque<protos::TestEvent> Globals::eventQueue = std::deque<protos::TestEvent>();

char Globals::currentHeader[4] = "";

std::uint32_t Globals::ID = 0;