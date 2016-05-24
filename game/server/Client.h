#pragma once

#include <memory>

#include "util\Protos.pb.h"

typedef int ClientId;

class Client {
public:
	virtual ~Client() {}
	virtual void deliver(const protos::Message msg) = 0;

	int getClientId() {
		return clientId_;
	}

	void setClientId(ClientId clientId) {
		clientId_ = clientId;
	}

private:
	ClientId clientId_;
};

typedef std::shared_ptr<Client> client_ptr;
