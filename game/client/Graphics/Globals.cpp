#include "Globals.h"

Camera Globals::camera = Camera();

Cube Globals::cube = Cube(10.0);

Light Globals::light = Light();

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();

boost::asio::io_service Globals::io_service = boost::asio::io_service();
tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<char> Globals::keyQueue = std::deque<char>();

char Globals::currentHeader[4] = "";