#pragma once

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Globals
{

public:
	static boost::asio::io_service io_service;
	static tcp::socket socket;
};