#pragma once

#include <deque>
#include <memory>
#include "Client.h"
#include "Game.h"

#include <boost/asio.hpp>
#include "util\Protos.pb.h"
#include "util\Util.h"

using boost::asio::ip::tcp;
using namespace util;

typedef std::deque<protos::TestEvent> message_queue;


class Session
	: public Client,
	public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket socket, Game& game);
	void start();
	void deliver(const protos::TestEvent msg);

private:
	void do_read_header();
	void do_read_body(size_t length);
	void do_write();

	tcp::socket socket_;
	Game& game_;
	char current_header_[4];
	message_queue write_msgs_;
};