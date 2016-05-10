#pragma once

#include <boost/asio.hpp>
#include <deque>
#include "Graphics\DrawData.h"
#include "Graphics\UpdateData.h"
#include "Graphics\Camera.h"
#include "Graphics\Model.h"
#include "Graphics\Skybox.h"
#include "Graphics\LightShader.h"

#include "util\Protos.pb.h"
#include "util\Util.h"

using boost::asio::ip::tcp;

class Globals {
public:
	static DrawData drawData;
	static UpdateData updateData;
	static Camera camera;
	static LightShader* lightShader;
	static Skybox skybox;

	static boost::asio::io_service io_service;
	static tcp::socket socket;
	static std::deque<protos::TestEvent> eventQueue;
	static char currentHeader[util::HEADER_SIZE];
	static std::uint32_t ID;
};

