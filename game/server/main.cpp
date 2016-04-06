//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "util\Message.h"

using boost::asio::ip::tcp;
using namespace util;


typedef std::deque<Message> message_queue;


class Client
{
public:
	virtual ~Client() {}
	virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<Client> client_ptr;


class Game
{
public:
	void join(client_ptr participant)
	{
		participants_.insert(participant);
	}


	void deliver(const Message& msg)
	{
		for (auto participant : participants_)
			participant->deliver(msg);
	}

private:
	std::set<client_ptr> participants_;
};


class session
	: public Client,
	public std::enable_shared_from_this<session> {
public:
	session(tcp::socket socket, Game& game)
		: socket_(std::move(socket)), 
		game_(game) {
	}

	void start() {
		game_.join(shared_from_this());
		do_read_header();
	}

	void deliver(const Message& msg) {
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress) {
			do_write();
		}
	}

private:
	void do_read_header() {
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), Message::header_length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && read_msg_.decode_header()) {
				do_read_body();
			}
		});
	}


	void do_read_body() {
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec) {
				game_.deliver(read_msg_);
				do_read_header();
			}
		});
	}

	void do_write() {
		auto self(shared_from_this());
		boost::asio::async_write(socket_,
			boost::asio::buffer(write_msgs_.front().data(), 
			write_msgs_.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				write_msgs_.pop_front();
				if (!write_msgs_.empty()) {
					do_write();
				}
			}
		});
	}

	tcp::socket socket_;
	Game& game_;
	Message read_msg_;
	message_queue write_msgs_;
};

class server {
public:
	server(boost::asio::io_service& io_service, short port)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
		socket_(io_service) {
		do_accept();
	}

private:
	void do_accept() {
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec) {
			if (!ec) {
				std::make_shared<session>(std::move(socket_), game_)->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
	Game game_;
};

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		server s(io_service, std::atoi(argv[1]));

		io_service.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}