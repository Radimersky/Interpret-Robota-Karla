#ifndef KAREL_COMFUNCTIONS_HPP
#define KAREL_COMFUNCTIONS_HPP

#pragma once
#include <sstream>
#include <string>
#include <vector>

class comFunctions {
public:
	enum class validInstrEnum { DEFINE,
		END,
		ELSE,
		SKIP,
		BREAK,
		HALT,
		MOVE,
		LEFT,
		RIGHT,
		PICKUP,
		PUTDOWN,
		IFWALL,
		IFMARK,
		UNDEFINED };

	static validInstrEnum instrStringToEnum(const std::string &instr) {
		if (instr == "DEFINE")
			return validInstrEnum::DEFINE;
		if (instr == "END")
			return validInstrEnum::END;
		if (instr == "ELSE")
			return validInstrEnum::ELSE;
		if (instr == "SKIP")
			return validInstrEnum::SKIP;
		if (instr == "BREAK")
			return validInstrEnum::BREAK;
		if (instr == "HALT")
			return validInstrEnum::HALT;
		if (instr == "MOVE")
			return validInstrEnum::MOVE;
		if (instr == "LEFT")
			return validInstrEnum::LEFT;
		if (instr == "RIGHT")
			return validInstrEnum::RIGHT;
		if (instr == "PICKUP")
			return validInstrEnum::PICKUP;
		if (instr == "PUTDOWN")
			return validInstrEnum::PUTDOWN;
		if (instr == "IFWALL")
			return validInstrEnum::IFWALL;
		if (instr == "IFMARK")
			return validInstrEnum::IFMARK;

		return validInstrEnum::UNDEFINED;
	}
	/**
	 * Clears tokensVec and fills it with tokens of words from inputStr (' ' is delimiter)
	 * @param inputStr to parse
	 * @param tokensVec to fill with words
	 */

	static void getWordsFromString(std::string &inputStr, std::vector<std::string> &tokensVec) {
		std::string buf; // String buffer
		std::stringstream ss(inputStr); // Insert the string into a stream
		tokensVec.clear();
		while (ss >> buf) {
			tokensVec.push_back(buf);
		}
	}

private:
	comFunctions() = default;
};

#endif //KAREL_COMFUNCTIONS_HPP
