#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

#include "util\ConfigSettings.h"
#include "util\Protos.pb.h"
#include "util\Util.h"

using boost::asio::ip::tcp;
using namespace util;


typedef std::deque<protos::TestEvent> message_queue;


class Client {
public:
	virtual ~Client() {}
	virtual void deliver(const protos::TestEvent msg) = 0;
};

typedef std::shared_ptr<Client> client_ptr;


class Game {
public:
	Game(int requiredPlayers)
		: requiredPlayers_(requiredPlayers),
		gameStarted_(false) {
	}

	void join(client_ptr client) {
		clients_.insert(client);
	}

	void remove(client_ptr client) {
		clients_.erase(client);
	}

	void deliver(protos::TestEvent msg) {
		//if (clients_.size() >= requiredPlayers_) {
		//	if (!gameStarted_) {
		//		initGame();
		//		gameStarted_ = true;
		//	}

		//	for (auto client : clients_)
		//		client->deliver(msg);
		//}
		for (auto client : clients_) {
			client->deliver(msg);
		}
	}

	int size() {
		return clients_.size();
	}

private:
	void initGame() {
		gameStarted_ = true;
	}

	std::set<client_ptr> clients_;
	int requiredPlayers_;
	bool gameStarted_;
};


class Session
	: public Client,
	public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket socket, Game& game)
		: socket_(std::move(socket)),
		game_(game) {
	}

	void start() {
		game_.join(shared_from_this());
		protos::TestEvent event;
		event.set_clientid(game_.size());
		event.set_type(protos::TestEvent_Type_ASSIGN);
		
		sendEvent(socket_, event);
		do_read_header();
	}

	void deliver(const protos::TestEvent msg) {
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


	void do_read_body(size_t length) {
		auto self(shared_from_this());
		char* body = new char[length];
		socket_.async_read_some(boost::asio::buffer(body, length),
			[this, self, body, length](boost::system::error_code ec, std::size_t) {
			if (!ec) {
				protos::TestEvent event;
				event.ParseFromArray(body, length);
				game_.deliver(event);
				delete body;
				do_read_header();
			}
			else {
				game_.remove(shared_from_this());
			}
		});
	}

	void do_write() {
		auto self(shared_from_this());

		protos::TestEvent event = write_msgs_.front();

		auto messageSize = event.ByteSize();

		if (messageSize > MAX_MESSAGE_SIZE) {
			return;
		}

		std::uint8_t message[HEADER_SIZE + MAX_MESSAGE_SIZE] = {};
		encode_header(messageSize, message);
		event.SerializeToArray(&message[HEADER_SIZE], messageSize);

		boost::asio::async_write(socket_,
			boost::asio::buffer(message, messageSize + HEADER_SIZE),
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
	char current_header_[4];
	message_queue write_msgs_;
};

class Server {
public:
	Server(boost::asio::io_service& io_service, short port, int requiredPlayers)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
		socket_(io_service), game_(requiredPlayers) {
		do_accept();
	}

private:
	void do_accept() {
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec) {
			if (!ec) {
				std::make_shared<Session>(std::move(socket_), game_)->start();
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

		// load the settings from the config file
		if (!ConfigSettings::config->checkIfLoaded()) {
			if (!ConfigSettings::config->loadSettingsFile()) {
				std::cerr << "There was a problem loading the config file\n";
				return 1;
			}
		}

		int port;
		// set the port
		if (!ConfigSettings::config->getValue(ConfigSettings::str_port_number, port)) {
			std::cerr << "There was a problem getting the port number from the config file\n";
			return 1;
		}

		int requiredPlayers;
		if (!ConfigSettings::config->getValue(ConfigSettings::str_required_players, requiredPlayers)) {
			std::cerr << "There was a problem getting the number of required players from the config file\n";
			return 1;
		}

		boost::asio::io_service io_service;

		Server s(io_service, port, requiredPlayers);

		io_service.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}