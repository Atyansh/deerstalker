#include "Position.h"

namespace util {
	Position::Position() : x(0.0), y(0.0), z(0.0) {
	}

	Position::Position(double x, double y, double z) : x(x), y(y), z(z) {
	}

	Position::~Position() {
	}
}