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
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "util\ConfigSettings.h"

using boost::asio::ip::tcp;

class session
	: public std::enable_shared_from_this<session> {
public:
	session(tcp::socket socket)
		: socket_(std::move(socket)) {
	}

	void start() {
		do_read();
	}

private:
	void do_read() {
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec) {
				do_write(length);
			}
		});
	}

	void do_write(std::size_t length) {
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				do_read();
			}
		});
	}

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
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
				std::make_shared<session>(std::move(socket_))->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
};

int main(int argc, char* argv[]) {
	int port;

	try {
		// load the settings from the config file
		if ( ! util::ConfigSettings::config->checkIfLoaded()) {
			if ( ! util::ConfigSettings::config->loadSettingsFile()) {
				std::cerr << "There was a problem loading the config file\n";
				return 1;
			}
		}
		// set the port
		if ( ! util::ConfigSettings::config->getValue(util::ConfigSettings::str_port_number, port)) {
			std::cerr << "There was a problem getting the port number from the config file\n";
			return 1;
		}

		boost::asio::io_service io_service;

		server s(io_service, port);

		io_service.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}