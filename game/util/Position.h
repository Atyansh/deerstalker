#pragma once
namespace util {
	class Position {
	public:
		Position();
		Position(double x, double y, double z);
		~Position();
		
		double x, y, z;
	};
}