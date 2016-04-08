#pragma once

#include <boost/asio.hpp>
#include <deque>

using boost::asio::ip::tcp;

class Globals {
public:
	static boost::asio::io_service io_service;
	static tcp::socket socket;

	static std::deque<char> keyQueue;

	static char currentHeader[4];
};

