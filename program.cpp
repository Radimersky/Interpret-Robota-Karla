#include "program.hpp"

bool operator==(const Procedure &first, const std::string &second) {
	return first.name == second;
}

bool operator!=(const Procedure &first, const std::string &second) {
	return !(first.name == second);
}

bool operator==(const std::string &first, const Procedure &second) {
	return first == second.name;
}

bool operator!=(const std::string &first, const Procedure &second) {
	return !(first == second.name);
}