#ifndef KAREL_WORLD_HPP
#define KAREL_WORLD_HPP

#pragma once
#include "defs.hpp"
#include "robot.hpp"

class World {
private:
	size_t _width;
	size_t _height;
	std::vector<Tile> tiles;
	Robot robot;
	friend class ParserWorld;
	friend class Interpret;

public:
	size_t height() const;
	size_t width() const;
	Tile tile(Position where) const;
	Tile& getTileRef(Position where);
};

#endif //KAREL_WORLD_HPP
