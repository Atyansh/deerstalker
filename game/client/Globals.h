#pragma once

#include <boost/asio.hpp>
#include <deque>
#include "Graphics\DrawData.h"
#include "Graphics\UpdateData.h"
#include "Graphics\Camera.h"
#include "Graphics\Camera_Original.h"
#include "Graphics\Model.h"
#include "GameObjects.h"

#include "util\Protos.pb.h"
#include "util\Util.h"

#include <mutex>

using boost::asio::ip::tcp;

class Globals {
public:
	static DrawData drawData;
	static UpdateData updateData;
	static Camera_Original camera;
	static Camera cam;
	static GameObjects gameObjects;

	static boost::asio::io_service io_service;
	static tcp::socket socket;
	static char currentHeader[util::HEADER_SIZE];
	static std::uint32_t ID;
};

