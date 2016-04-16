#include "Globals.h"

boost::asio::io_service Globals::io_service = boost::asio::io_service();
tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<protos::TestEvent> Globals::eventQueue = std::deque<protos::TestEvent>();

char Globals::currentHeader[4] = "";

std::uint32_t Globals::ID = 0;