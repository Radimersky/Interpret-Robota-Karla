#ifndef KAREL_PARSERPROGRAM_HPP
#define KAREL_PARSERPROGRAM_HPP

#pragma once
#include "program.hpp"
#include <map>
#include <set>

class ParserProgram {
private:
	Program parsedProg;
	std::istream &inputProg;
	unsigned int currentLine = 0;
	std::set<std::string> validInstructions{ "DEFINE", "END", "ELSE", "SKIP", "BREAK", "HALT", "MOVE", "LEFT", "RIGHT",
		"PICKUP", "PUTDOWN", "IFWALL", "IFMARK" };
	std::set<std::string> builtInInst{ "SKIP", "BREAK", "HALT", "MOVE", "LEFT", "RIGHT", "PICKUP", "PUTDOWN" };
	std::set<std::string> forbiddenJumpInstr{ "DEFINE", "END", "ELSE", "IFWALL", "IFMARK" };
	std::map<std::string, std::string> procedureCalls;

	void selectInstrInserter(const std::vector<std::string> &lineWords, bool &isInsideProcedure, std::string &procedureName);
	void insertDEFINE(const std::vector<std::string> &lineWords, bool &isInsideProcedure, std::string &procedureName);
	static inline bool charIsNotAlpfaNum(char c);
	bool stringIsAlpfaNum(const std::string &str);
	void checkENDInstCorrectness(const std::vector<std::string> &lineWords, bool &isInsideProcedure);
	void insertBuiltInInstr(const std::vector<std::string> &lineWords, bool &isInsideProcedure, const std::string &procedureName);
	void insertJumpInstr(const std::vector<std::string> &lineWords, bool &isInsideProcedure, const std::string &procedureName);
	void insertIntoProcBody(const std::vector<std::string> &lineWords, const std::string &procedureName, bool isProcCall, int procCallIndex = -1);
	void checkElse(const std::vector<std::string> &lineWords, bool &isInsideProcedure,
	    std::string &procedureName);
	/**
     * Checks if every procedure call have defined procedure. If not, complaint is thrown
     */
	void checkIfCalledProcedExists();

public:
	ParserProgram(std::istream &in)
	    : inputProg(in) {}
	Program &parseProgram();
};

#endif //KAREL_PARSERPROGRAM_HPP
