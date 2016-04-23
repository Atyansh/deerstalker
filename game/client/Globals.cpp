#include "Globals.h"

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
Camera Globals::camera = Camera();

boost::asio::io_service Globals::io_service = boost::asio::io_service();
tcp::socket Globals::socket = tcp::socket(Globals::io_service);

std::deque<protos::Message> Globals::messageQueue = std::deque<protos::Message>();

char Globals::currentHeader[4] = "";

uint32_t Globals::ID = 0;

float Globals::AXIS_TILT = 0.5;

float Globals::NEG_AXIS_TILT = -AXIS_TILT;
float Globals::POS_AXIS_TILT = AXIS_TILT;

int Globals::GAMEPAD_LEFT_STICK_X = 0;
int Globals::GAMEPAD_LEFT_STICK_Y = 1;
int Globals::GAMEPAD_RIGHT_STICK_X = 3;
int Globals::GAMEPAD_RIGHT_STICK_Y = 4;
int Globals::GAMEPAD_TRIGGER_AXIS = 2;