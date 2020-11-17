#include "robot.hpp"

Position Robot::position() const {
	return robotPos;
}

Direction Robot::direction() const {
	return robotDir;
}

void Robot::setPosition(int posX, int posY) {
	robotPos.x = posX;
	robotPos.y = posY;
}

void Robot::setDirection(Direction dir) {
	robotDir = dir;
}

void Robot::setPosition(Position &pos) {
	robotPos.x = pos.x;
	robotPos.y = pos.y;
}
