#pragma once

#include <memory>

#include "util\Protos.pb.h"

typedef int ClientId;

class Client {
public:
	virtual ~Client() {}
	virtual void deliver(const protos::TestEvent msg) = 0;

	int getClientId() {
		return clientId_;
	}

private:
	ClientId clientId_;
};

typedef std::shared_ptr<Client> client_ptr;
