#include "Server.h"

Server::Server(boost::asio::io_service& io_service, short port, Game& game)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service), game_(game) {
	do_accept();
}

void Server::do_accept() {
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec) {
		if (!ec) {
			boost::asio::socket_base::receive_buffer_size option(1000000);
			socket_.set_option(option);
			socket_.set_option(tcp::no_delay(true));
			std::make_shared<Session>(std::move(socket_), game_)->start();
		}

		do_accept();
	});
}