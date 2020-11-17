#ifndef KAREL_ROBOT_HPP
#define KAREL_ROBOT_HPP

#pragma once
#include "defs.hpp"

class Robot {
private:
	Direction robotDir;
	Position robotPos;

public:
	void setPosition(int posX, int posY);
	void setPosition(Position& pos);
	void setDirection(Direction dir);
	Position position() const;
	Direction direction() const;
};

#endif //KAREL_ROBOT_HPP
