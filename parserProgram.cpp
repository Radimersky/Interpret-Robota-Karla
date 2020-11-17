#include "parserProgram.hpp"
#include "comFunctions.hpp"
#include "errors.hpp"
#include <algorithm>

bool ParserProgram::charIsNotAlpfaNum(char c) {
	return !(isalnum(c));
}

bool ParserProgram::stringIsAlpfaNum(const std::string &str) {
	return find_if(str.begin(), str.end(), charIsNotAlpfaNum) == str.end();
}

Program &ParserProgram::parseProgram() {
	parsedProg = Program(); // init Program
	std::vector<std::string> lineWords; // For tokenized line
	std::string line;
	bool isInsideProcedure = false; // To controll if instructions are inside procedure
	std::string currentProcedureName; // For knowing where parsed instructions belong

	while (getline(inputProg, line)) { // Parse lines
		if (inputProg.fail()) {
			Complain::invalidSource(currentLine, "error reading from file");
		}
		currentLine++; //increment line number
		comFunctions::getWordsFromString(line, lineWords); // Split line to words
		if (lineWords.empty() || lineWords[0][0] == '#') { // Skip line if is empty or is comment
			continue;
		}
		// lineWords[0] represents first string in line
		if (validInstructions.find(lineWords[0]) != validInstructions.end()) { // If instruction is valid, insert it into vector of instructions
			selectInstrInserter(lineWords, isInsideProcedure, currentProcedureName); // Insert instruction that is not procedure call
		} else if (stringIsAlpfaNum(lineWords[0]) && isInsideProcedure && lineWords.size() == 1) { // If instruction is valid procedure call and is inside procedure
			insertIntoProcBody(lineWords, currentProcedureName, true, 0); // Insert instruction that is procedure call
		} else {
			Complain::invalidSource(currentLine, "line:(", line, ") is invalid");
		}
	}

	checkIfCalledProcedExists(); // Checks if every procedure call have defined procedure and if MAIN is defined. If not, complaint is thrown

	if (isInsideProcedure) { // Ensures that last parsed procedure contains END instruction
		Complain::invalidSource(currentLine + 1, "END instruction is missing");
	}
	return parsedProg;
}

void ParserProgram::selectInstrInserter(const std::vector<std::string> &lineWords, bool &isInsideProcedure,
    std::string &procedureName) {
	comFunctions::validInstrEnum instr = comFunctions::instrStringToEnum(lineWords[0]);
	switch (instr) {
	case comFunctions::validInstrEnum::DEFINE:
		insertDEFINE(lineWords, isInsideProcedure, procedureName);
		break;
	case comFunctions::validInstrEnum::END:
		checkENDInstCorrectness(lineWords, isInsideProcedure);
		break;
	case comFunctions::validInstrEnum::SKIP:
	case comFunctions::validInstrEnum::BREAK:
	case comFunctions::validInstrEnum::HALT:
	case comFunctions::validInstrEnum::MOVE:
	case comFunctions::validInstrEnum::LEFT:
	case comFunctions::validInstrEnum::RIGHT:
	case comFunctions::validInstrEnum::PICKUP:
	case comFunctions::validInstrEnum::PUTDOWN:
		insertBuiltInInstr(lineWords, isInsideProcedure, procedureName);
		break;
	case comFunctions::validInstrEnum::IFWALL:
	case comFunctions::validInstrEnum::IFMARK:
		insertJumpInstr(lineWords, isInsideProcedure, procedureName);
		break;
	case comFunctions::validInstrEnum::ELSE:
		checkElse(lineWords, isInsideProcedure, procedureName);
		break;
	case comFunctions::validInstrEnum::UNDEFINED:
		break;
	}
}

void ParserProgram::insertDEFINE(const std::vector<std::string> &lineWords, bool &isInsideProcedure,
    std::string &procedureName) {

	// If instruction is DEFINE, it cant be inside procedure and it has to have name that consists alfa numeric chars only and the name cant exist already or cant be named same as instruction.
	if (isInsideProcedure) {
        //todo: tady mozna spatny hlaseny radek
		Complain::invalidSource(currentLine - 1, "DEFINE has no END");
	}

	if (lineWords.size() == 2 && stringIsAlpfaNum(lineWords[1])) {
		for (auto &i : parsedProg.procedures) { // Check if name of DEFINE does not exist already
			if (lineWords[1] == i.name) {
				Complain::invalidSource(currentLine, "DEFINE name already exists");
			}
		}
		for (auto &i : validInstructions) { // Check if name of DEFINE is not same as instruction names
			if (lineWords[1] == i) {
				Complain::invalidSource(currentLine, "DEFINE name is named as instruction");
			}
		}
		isInsideProcedure = true;
		procedureName = lineWords[1]; // Now we know where to put instructions inside this procedure
		parsedProg.procedures.emplace_back(); // Insert procedure
		parsedProg.procedures.back().name = lineWords[1]; // Set proc. name
	} else {
		Complain::invalidSource(currentLine, "DEFINE name is not alphanumeric");
	}
}

void ParserProgram::checkENDInstCorrectness(const std::vector<std::string> &lineWords, bool &isInsideProcedure) {
	if (isInsideProcedure && lineWords.size() == 1) {
		isInsideProcedure = false;
	} else {
		Complain::invalidSource(currentLine, "END has no DEFINE");
	}
}

void ParserProgram::insertBuiltInInstr(const std::vector<std::string> &lineWords, bool &isInsideProcedure,
    const std::string &procedureName) {
	if (isInsideProcedure && lineWords.size() == 1) {
		insertIntoProcBody(lineWords, procedureName, false);
	} else {
		Complain::invalidSource(currentLine, "Parameter is not inside procedure or parameter is missing or there are too many of them");
	}
}

void ParserProgram::insertJumpInstr(const std::vector<std::string> &lineWords, bool &isInsideProcedure,
    const std::string &procedureName) {
	// Instruction must be inside procedure and has to have exactly one parameter and the parameter must be alphanumeric
	if (isInsideProcedure && lineWords.size() == 2 && stringIsAlpfaNum(lineWords[1])) {
		if (forbiddenJumpInstr.find(lineWords[1]) != forbiddenJumpInstr.end()) {
			Complain::invalidSource(currentLine, "Forbidden instruction name");
		}
		if (builtInInst.find(lineWords[1]) != builtInInst.end()) { // If instruction does not call procedure
			insertIntoProcBody(lineWords, procedureName, false);
		} else {
			insertIntoProcBody(lineWords, procedureName, true, 1);
		}
	} else {
		Complain::invalidSource(currentLine, "Instruction is not inside procedure or does not have one parameter or is not alphanumeric");
	}
}

void ParserProgram::insertIntoProcBody(const std::vector<std::string> &lineWords, const std::string &procedureName, bool isProcCall, int procCallIndex) {
	if (isProcCall) {
		if (procedureCalls.find(lineWords[procCallIndex]) == procedureCalls.end()) { // Check if procedure is already in map. If not, insert it. Key = procedure call and Value = Name of procedure where it is called from
			procedureCalls[lineWords[procCallIndex]] = procedureName; // Later we can check if called procedure actually exists
		}
	}

	for (auto &procedure : parsedProg.procedures) {
		if (procedure.name == procedureName) {
			procedure.instructions.emplace_back();
			procedure.instructions.back().inst1 = lineWords[0];
			if (lineWords.size() == 2) {
				procedure.instructions.back().inst2 = lineWords[1];
			}
			return;
		}
	}

	Complain::error("insertIntoProcBody failed finding given procedure name");
}

void ParserProgram::checkIfCalledProcedExists() {
	bool mainIsDefined = false;
	for (auto &i : parsedProg.procedures) { // Check if MAIN is defined
		if (i.name == "MAIN") {
			mainIsDefined = true;
		}
	}

	if (!mainIsDefined) {
		Complain::undefinedReference("@ENTRY", "MAIN", "Procedure MAIN is not defined");
	}

	std::map<std::string, std::string>::iterator it;

	for (it = procedureCalls.begin(); it != procedureCalls.end(); ++it) { // Check if rest of procedure calls are defined
		if (std::find(parsedProg.procedures.begin(), parsedProg.procedures.end(), it->first) == parsedProg.procedures.end()) {
			Complain::undefinedReference(it->second, it->first, "Procedure call is not defined");
		}
	}
}

void ParserProgram::checkElse(const std::vector<std::string> &lineWords, bool &isInsideProcedure, std::string &procedureName) {
	if (!parsedProg.procedures.back().instructions.empty() && (parsedProg.procedures.back().instructions.back().inst1 == "IFMARK" || parsedProg.procedures.back().instructions.back().inst1 == "IFWALL")) { // Check if ELSE is after IFMARK or IFWALL
		insertJumpInstr(lineWords, isInsideProcedure, procedureName);
	} else {
		Complain::invalidSource(currentLine, "ELSE is not after IFWALL or IFMARK");
	}
}
