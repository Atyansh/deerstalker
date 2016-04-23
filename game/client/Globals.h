#pragma once

#include <boost/asio.hpp>
#include <deque>
#include "Graphics\DrawData.h"
#include "Graphics\UpdateData.h"
#include "Graphics\Camera.h"
#include "Graphics\Model.h"

#include "util\Protos.pb.h"

using boost::asio::ip::tcp;

class Globals {
public:
	static DrawData drawData;
	static UpdateData updateData;
	static Camera camera;
	static boost::asio::io_service io_service;
	static tcp::socket socket;
	static std::deque<protos::Message> messageQueue;
	static char currentHeader[4];

	static uint32_t ID;
	static float NEG_AXIS_TILT;
	static float POS_AXIS_TILT;

	static int GAMEPAD_LEFT_STICK_X;
	static int GAMEPAD_LEFT_STICK_Y;
	static int GAMEPAD_RIGHT_STICK_X;
	static int GAMEPAD_RIGHT_STICK_Y;
	static int GAMEPAD_TRIGGER_AXIS;

private:
	static float AXIS_TILT;
};

