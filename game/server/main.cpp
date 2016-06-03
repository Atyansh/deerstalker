#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

#include "util\ConfigSettings.h"
#include "util\Protos.pb.h"
#include "util\Util.h"

#include "Server.h"

using boost::asio::ip::tcp;
using namespace util;


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

		boost::asio::io_service io_service;

		Game game;

		Server s(io_service, port, game);
		std::thread t([&io_service](){ io_service.run(); });

		game.initialize();
		game.startGameLoop();

		t.join();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}