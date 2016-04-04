#pragma once

#include "Globals.h"
#include "util\Position.h"

namespace common {
	class Entity {
	public:
		Entity(int id);
		Entity(int id, util::Position position);
		~Entity();
		int getId();
		util::Position getPosition();
		void Entity::setPosition(util::Position position);
		void update();
	private:
		int id_;
		util::Position position_;
	};
}