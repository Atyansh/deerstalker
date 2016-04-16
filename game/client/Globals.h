#pragma once

#include <boost/asio.hpp>
#include <deque>

#include "util\Protos.pb.h"

using boost::asio::ip::tcp;

class Globals {
public:
	static boost::asio::io_service io_service;
	static tcp::socket socket;

	static std::deque<protos::TestEvent> eventQueue;

	static char currentHeader[4];

	static std::uint32_t ID;
};