#include "interpret.hpp"
#include "comFunctions.hpp"

bool Interpret::running() const {
	return isInterRunning;
}

bool Interpret::hasProcedure(const std::string& name) const {
	for (auto& i : _program.procedures) {
		if (i.name == name) {
			return true;
		}
	}
	return false;
}

const Robot& Interpret::robot() const {
	return _world.robot;
}

const World& Interpret::world() const {
	return _world;
}

bool Interpret::step() {
	if (isInterRunning) {
		if (procedureStack.top() == findProcByName(procedureStackNames.top()).instructions.end()) { // If procedure has no more instructions
			popStack();
			if (procedureStack.empty()) { // If program has no more instructions to handle
				isInterRunning = false;
				return false;
			}
			return true;
		}
		selectInstruction(procedureStack.top()->inst1); // Perform instruction

		if (!procWasCalledOrEnded) { // Move to next instruction if we are still processing the same procedure (no procedure call or end)
			procedureStack.top()++;
		} else {
			procWasCalledOrEnded = false; // Unset flag
		}
		return true;
	}

     if (procedureStack.empty()) { // If program has no more instructions to handle
				isInterRunning = false;
				return false;
			}


	return false; // If interpret is not running
}

void Interpret::turnRobotLeft() {
	switch (_world.robot.direction()) {
	case Direction::East:
		_world.robot.setDirection(Direction::North);
		break;
	case Direction::North:
		_world.robot.setDirection(Direction::West);
		break;
	case Direction::West:
		_world.robot.setDirection(Direction::South);
		break;
	default:
		_world.robot.setDirection(Direction::East);
	}
}

void Interpret::turnRobotRight() {
	switch (_world.robot.direction()) {
	case Direction::East:
		_world.robot.setDirection(Direction::South);
		break;
	case Direction::South:
		_world.robot.setDirection(Direction::West);
		break;
	case Direction::West:
		_world.robot.setDirection(Direction::North);
		break;
	default:
		_world.robot.setDirection(Direction::East);
	}
}

void Interpret::pickUp() {
	if (_world.tile(_world.robot.position()).marks == 0) { // If marks == 0
		Complain::runtimeError(procedureStackNames.top(), static_cast<unsigned int>(procedureStack.top() - findProcByName(procedureStackNames.top()).instructions.begin()), "Robot have no sign to pick"); // second param gets index of array from iterator
	}
	_world.getTileRef(_world.robot.position()).marks--;
}

Procedure& Interpret::findProcByName(const std::string& name) {
	for (auto& i : _program.procedures) {
		if (i.name == name) {
			return i;
		}
	}
	Complain::error("procedure not found");
}

void Interpret::moveRobot() {
	Position futurePos; // this gets set in the checkwall() method
	if (checkIfWallNextToRobot(futurePos)) { // If robot steps to wall
		Complain::runtimeError(procedureStackNames.top(), static_cast<unsigned int>(procedureStack.top() - findProcByName(procedureStackNames.top()).instructions.begin()), "Robot stepped on forbidden place");
	}

	_world.robot.setPosition(futurePos); // Move robot
}

void Interpret::putDown() {
	_world.getTileRef(_world.robot.position()).marks++;
}

bool Interpret::checkIfWallNextToRobot(Position& fillPosition) {
	fillPosition = _world.robot.position();
	bool xPosChanged = false;

	switch (_world.robot.direction()) {
	case Direction::South:
		fillPosition.y++;
		break;
	case Direction::North:
		fillPosition.y--;
		break;
	case Direction::East:
		fillPosition.x++;
		xPosChanged = true;
		break;
	default:
		fillPosition.x--;
		xPosChanged = true;
		break;
	}

	if (xPosChanged) { // Check boundaries
		if (fillPosition.x >= static_cast<int>(_world.width()) || fillPosition.x < 0) {
			return true;
		}
	} else if (fillPosition.y >= static_cast<int>(_world.height()) || fillPosition.y < 0) {
		return true;
	}

	return _world.tile(fillPosition).wall; // returns true, if there is a wall
}

void Interpret::ifWall() {
	Position futurePos; // this gets set in the checkwall() method
	if (checkIfWallNextToRobot(futurePos)) { // If wall is in front of robot
		_performELSE = false;
		selectInstruction(procedureStack.top()->inst2); // Perform instruction
	} else {
		_performELSE = true;
	}
}

void Interpret::selectInstruction(const std::string& instruction) {
	comFunctions::validInstrEnum instr = comFunctions::instrStringToEnum(instruction);
	switch (instr) {
	case comFunctions::validInstrEnum::SKIP:
		//Do nothing
		break;
	case comFunctions::validInstrEnum::BREAK:
		breakProc();
		break;
	case comFunctions::validInstrEnum::HALT:
		performHalt();
		break;
	case comFunctions::validInstrEnum::MOVE:
		moveRobot();
		break;
	case comFunctions::validInstrEnum::LEFT:
		turnRobotLeft();
		break;
	case comFunctions::validInstrEnum::RIGHT:
		turnRobotRight();
		break;
	case comFunctions::validInstrEnum::PICKUP:
		pickUp();
		break;
	case comFunctions::validInstrEnum::PUTDOWN:
		putDown();
		break;
	case comFunctions::validInstrEnum::IFWALL:
		ifWall();
		break;
	case comFunctions::validInstrEnum::IFMARK:
		ifMark();
		break;
	case comFunctions::validInstrEnum::ELSE:
		performELSE();
		break;
	default:
		performProcCall(instruction);
	}
}

void Interpret::breakProc() {
	popStack(); // Break from procedure
	procWasCalledOrEnded = true;
	if (procedureStack.empty()) {
		isInterRunning = false;
	}
}

void Interpret::ifMark() {
	if (_world.tile(_world.robot.position()).marks > 0) { // If robot stands on at least one mark
		_performELSE = false;
		selectInstruction(procedureStack.top()->inst2); // Perform instruction
	} else {
		_performELSE = true;
	}
}

void Interpret::performELSE() {
	if (_performELSE) {
		selectInstruction(procedureStack.top()->inst2); // Perform instruction
		_performELSE = false;
	}
}

void Interpret::performProcCall(const std::string& name) {
	procedureStack.top()++; // set current procedure iterator to next element
	pushStack(findProcByName(name));
	procWasCalledOrEnded = true;
}

unsigned Interpret::run() {
	if (isInterRunning) {
		while (isInterRunning) {
			step();
			stepCounter++;
		}
	} else {
		return 0u;
	}
	std::cout << stepCounter << " steps\n";
	std::cout << _world.robot.position().x << " x\n";
	std::cout << _world.robot.position().y << " y\n";
	std::cout << _world.tile(_world.robot.position()).marks << " marks\n";
	if (_world.robot.direction() == Direction::West) {
		std::cout << "west direction \n";
	}
	if (_world.robot.direction() == Direction::East) {
		std::cout << "east direction \n";
	}
	if (_world.robot.direction() == Direction::North) {
		std::cout << "north direction \n";
	}
	if (_world.robot.direction() == Direction::South) {
		std::cout << "south direction \n";
	}
	return stepCounter;
}

void Interpret::popStack() {
	procedureStack.pop();
	procedureStackNames.pop();
}

void Interpret::pushStack(Procedure& proc) {
	procedureStack.push(proc.instructions.begin()); // Push called procedure to stack
	procedureStackNames.push(proc.name);
}

void Interpret::performHalt() {
	isInterRunning = false;
    while (!procedureStack.empty()) {
        procedureStack.pop();
    }

}
