#include "parserWorld.hpp"
#include "comFunctions.hpp"
#include "errors.hpp"

World &ParserWorld::parseWorld() {
	parsedWorld = World(); // Init world;

	currentLine++; //currentLine = 1
	parseAndAssignWorldSize(); // Parse and assign first line

	currentLine++; //currentLine = 2
	parseAndAssignRobotInformation(); // Parse and assign second line
	std::string line;
	while (getline(inputWorld, line)) { // Parse lines
		if (inputWorld.fail()) {
			Complain::invalidWorld(currentLine, "error reading from file");
		}
		checkForCorrectLine(line); // Check if line is correct
		for (unsigned int i = 0; i < parsedWorld.width(); ++i) { // Parse chars in line
			switch (line[i]) {
			case '#':
				parsedWorld.tiles.emplace_back();
				parsedWorld.tiles.back().wall = true;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				parsedWorld.tiles.emplace_back();
				parsedWorld.tiles.back().marks = static_cast<unsigned int>(line[i] - '0'); // converts char to number
				break;
			case '0':
			case ' ':
				parsedWorld.tiles.emplace_back();
				break;
			default:
				Complain::invalidWorld(currentLine, "Bad world symbol");
			}
		}
	}

	if (currentLine - 2 > parsedWorld.height()) { // Check correct world height
		Complain::invalidWorld(currentLine, "World is bigger than expected");
	}
	if (currentLine - 2 < parsedWorld.height()) {
		Complain::invalidWorld(currentLine + 1, "World is smaller than expected");
	}

	if (parsedWorld.tile(parsedWorld.robot.position()).wall) { // Check if robot's start position is inside wall
		Complain::invalidWorld(2, "Robot is inside wall");
	}

	return parsedWorld;
}

void ParserWorld::checkForCorrectDigitInput(const std::vector<std::string> &lineWords, unsigned int currentLine, unsigned int size) const {
	if (lineWords.size() == size) {
		try {
			stoi(lineWords[0]);
			stoi(lineWords[1]);
		} catch (std::invalid_argument &e) {
			Complain::invalidWorld(currentLine, "Non digit input");
		}
	} else {
		Complain::invalidWorld(currentLine, "Line does not contain two values");
	}

	if (stoi(lineWords[0]) < 0 || stoi(lineWords[1]) < 0) {
		Complain::invalidWorld(currentLine, "World size or robot position is negative value");
	}
}

void ParserWorld::parseAndAssignWorldSize() {
	std::vector<std::string> lineWords;
	std::string line;

	std::getline(inputWorld, line); // Get first line
	comFunctions::getWordsFromString(line, lineWords); // Fill vector lineWords with first line words
	checkForCorrectDigitInput(lineWords, currentLine, 2); // Check for correct input
	parsedWorld._width = static_cast<unsigned int>(stoi(lineWords[0])); // Set width of world
	parsedWorld._height = static_cast<unsigned int>(stoi(lineWords[1])); // Set height of world

	if (parsedWorld._width == 0 || parsedWorld._height == 0) {
		Complain::invalidWorld(1, "can't create world of size 0");
	}
}

void ParserWorld::parseAndAssignRobotInformation() {
	std::vector<std::string> lineWords;
	std::string line;

	std::getline(inputWorld, line); // Get first line
	comFunctions::getWordsFromString(line, lineWords); // Fill vector lineWords with second line words
	checkForCorrectDigitInput(lineWords, currentLine, 3); // Check for correct input
	parsedWorld.robot.setPosition(static_cast<unsigned int>(stoi(lineWords[0])), static_cast<unsigned int>(stoi(lineWords[1]))); // Set robot position
	if (static_cast<unsigned int>(parsedWorld.robot.position().x) > parsedWorld._width - 1 || static_cast<unsigned int>(parsedWorld.robot.position().y) > parsedWorld._height - 1) {
		Complain::invalidWorld(currentLine, "Robot is outside of the world");
	}

	switch (lineWords[2][0]) { // Assign robot direction
	case 'e':
		parsedWorld.robot.setDirection(Direction::East);
		break;
	case 'n':
		parsedWorld.robot.setDirection(Direction::North);
		break;
	case 's':
		parsedWorld.robot.setDirection(Direction::South);
		break;
	case 'w':
		parsedWorld.robot.setDirection(Direction::West);
		break;
	default:
		Complain::invalidWorld(currentLine, "Bad robot direction");
	}
}

void ParserWorld::checkForCorrectLine(const std::string &line) {
	if (!line.empty() && inputWorld.eof()) { // Check if last line is empty (every line must end with \n)
		Complain::invalidWorld(currentLine, "last line of world is missing \\n");
	}
	currentLine++; // currentLine > 2
	if (line.size() != parsedWorld.width()) { // Check if world size is correct
		Complain::invalidWorld(currentLine, "bad world size");
	}
}
