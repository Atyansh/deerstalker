#include "Globals.h"

boost::asio::io_service Globals::io_service = boost::asio::io_service();
tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<char> Globals::keyQueue = std::deque<char>();

char Globals::currentHeader[4] = "";
