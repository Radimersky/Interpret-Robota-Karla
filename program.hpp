#ifndef KAREL_PROGRAM_HPP
#define KAREL_PROGRAM_HPP

#pragma once
#include <set>
#include <string>
#include <vector>

struct Procedure;

struct Instruction {
	std::string inst1;
	std::string inst2;
};

struct Procedure {
	std::string name;
	std::vector<Instruction> instructions;
};

class Program {
private:
public:
	std::vector<Procedure> procedures;
};

bool operator!=(const Procedure &first, const std::string &second);
bool operator==(const Procedure &first, const std::string &second);
bool operator==(const std::string &first, const Procedure &second);
bool operator!=(const std::string &first, const Procedure &second);

#endif //KAREL_PROGRAM_HPP
