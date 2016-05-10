#include "Session.h"

using boost::asio::ip::tcp;
using namespace util;

Session::Session(tcp::socket socket, Game& game)
	: Client(), socket_(std::move(socket)),
	game_(game) {
}

void Session::start() {
	game_.join(shared_from_this());

	int clientId = game_.size();
	protos::Message message;
	auto* event = message.add_event();

	event->set_clientid(clientId);
	event->set_type(event->ASSIGN);
	sendMessage(socket_, message);
	message.Clear();
	event = message.add_event();
	event->set_clientid(clientId);
	event->set_type(event->SPAWN);
	game_.deliver(message);
	do_read_header();
}

void Session::deliver(const protos::Message msg) {
	bool write_in_progress = !write_msgs_.empty();
	write_msgs_.push_back(msg);
	if (!write_in_progress) {
		do_write();
	}
}

void Session::do_read_header() {
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
		boost::asio::buffer(current_header_, HEADER_SIZE),
		[this, self](boost::system::error_code ec, std::size_t /*length*/) {
		if (!ec) {
			int length = decode_header(current_header_);
			do_read_body(length);
		}
		else {
			game_.remove(shared_from_this());
		}
	});
}


void Session::do_read_body(size_t length) {
	auto self(shared_from_this());
	char* body = new char[length];
	socket_.async_read_some(boost::asio::buffer(body, length),
		[this, self, body, length](boost::system::error_code ec, std::size_t) {
		if (!ec) {
			protos::Message message;
			message.ParseFromArray(body, length);
			game_.deliver(message);
			delete body;
			do_read_header();
		}
		else {
			game_.remove(shared_from_this());
		}
	});
}

void Session::do_write() {
	auto self(shared_from_this());

	if (write_msgs_.empty()) {
		return;
	}
	protos::Message message = write_msgs_.front();

	std::uint8_t arr[HEADER_SIZE + MAX_MESSAGE_SIZE] = {};
	auto messageSize = fillMessage(arr, message);

	if (messageSize != 0) {
		boost::asio::async_write(socket_,
			boost::asio::buffer(arr, messageSize + HEADER_SIZE),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				if (!write_msgs_.empty()) {
					write_msgs_.pop_front();
				}
				if (!write_msgs_.empty()) {
					do_write();
				}
			}
		});
	}
}