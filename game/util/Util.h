#pragma once

#include <boost/asio.hpp>
#include "Protos.pb.h"
#include <iostream>

using boost::asio::ip::tcp;

namespace util {

	static const int MAX_MESSAGE_SIZE = 1000000;

	static const int HEADER_SIZE = 4;

	static size_t decode_header(const char* data) {
		char header[HEADER_SIZE + 1] = "";
		strncat_s(header, data, HEADER_SIZE);
		return std::atoi(header);
	}

	static void encode_header(size_t body_length, uint8_t* data)
	{
		char header[HEADER_SIZE + 1] = "";
		sprintf_s(header, "%4d", static_cast<int>(body_length));
		std::memcpy(data, header, HEADER_SIZE);
	}

	static int fillMessage(uint8_t* message, protos::TestEvent& event) {
		int messageSize = event.ByteSize();
		if (messageSize > MAX_MESSAGE_SIZE) {
			std::cerr << "MAX MESSAGE SIZE EXCEEDED" << std::endl;
			return 0;
		}
		encode_header(messageSize, message);
		event.SerializeToArray(&message[HEADER_SIZE], messageSize);

		return messageSize;
	}

	static void sendEvent(tcp::socket& socket, protos::TestEvent& event) {
		std::uint8_t message[HEADER_SIZE + MAX_MESSAGE_SIZE] = {};
		auto messageSize = fillMessage(message, event);
		if (messageSize != 0) {
			boost::asio::write(socket,
				boost::asio::buffer(message, messageSize + HEADER_SIZE));
		}
	}
}