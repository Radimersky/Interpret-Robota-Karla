#ifndef KAREL_PARSER_HPP
#define KAREL_PARSER_HPP

#pragma once
#include "world.hpp"
#include <iostream>

class ParserWorld {
private:
	std::istream& inputWorld;
	World parsedWorld;
	unsigned int currentLine = 0;

	/**
	 * Checks if vector size is at least 2 and if index 0 and 1 is digit. If not, complain exception is thrown
	 * @param lineWords vector with words to check
	 * @param currentLine is used when exception is thrown
	 * @param size to check correct size of vector
	 */
	void checkForCorrectDigitInput(const std::vector<std::string>& lineWords, unsigned int currentLine, unsigned int size) const;
	void parseAndAssignWorldSize();
	void parseAndAssignRobotInformation();
	/**
	 * Checks if given line is correct world description
	 * @param line to check
	 */
	void checkForCorrectLine(const std::string& line);

public:
	ParserWorld(std::istream& in)
	    : inputWorld(in) {}
	/**
	 * Parses world
	 * @return parsed world
	 */
	World& parseWorld();
};

#endif //KAREL_PARSER_HPP
