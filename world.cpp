#include "world.hpp"

size_t World::height() const {
	return _height;
}

size_t World::width() const {
	return _width;
}

Tile World::tile(Position where) const {
	return tiles[where.y * width() + where.x];
}

Tile &World::getTileRef(Position where) {
	return tiles[where.y * width() + where.x];
}
