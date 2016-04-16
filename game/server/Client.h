#pragma once

#include <memory>

#include "util\Protos.pb.h"

class Client {
public:
	virtual ~Client() {}
	virtual void deliver(const protos::TestEvent msg) = 0;
};

typedef std::shared_ptr<Client> client_ptr;

typedef int ClientId;