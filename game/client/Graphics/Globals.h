#pragma once

#include <boost/asio.hpp>
#include <deque>
#include "DrawData.h"
#include "UpdateData.h"
#include "Camera.h"

using boost::asio::ip::tcp;

class Globals {
public:
	static DrawData drawData;
	static UpdateData updateData;
	static Camera camera;

	static boost::asio::io_service io_service;
	static tcp::socket socket;
	static std::deque<char> keyQueue;
	static char currentHeader[4];
};

