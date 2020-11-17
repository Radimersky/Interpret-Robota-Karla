#ifndef KAREL_INTERPRET_HPP
#define KAREL_INTERPRET_HPP

#pragma once
#include "errors.hpp"
#include "parserProgram.hpp"
#include "parserWorld.hpp"
#include <stack>

class Interpret {
private:
	std::istream& inWorld;
	std::istream& inProgram;
	World _world;
	Program _program;
	bool isInterRunning = false;
	std::stack<std::vector<Instruction>::iterator> procedureStack;
	std::stack<std::string> procedureStackNames; // We dont know where instructions in procedureStack belong to, so when we push instruction iterator into procedureStack, we will also push procedure name into this stack
	unsigned int stepCounter = 0u;
	bool _performELSE = false; // Flag
	bool procWasCalledOrEnded = false; //Flag
	/**
     * Finds procedure by given name
     * @param name of procedure to find
     * @return reference to procedure
     */
	Procedure& findProcByName(const std::string& name);
	void turnRobotLeft();
	void turnRobotRight();
	void pickUp();
	void moveRobot();
	void putDown();
	void ifWall();
	void breakProc();
	void ifMark();
	void performELSE();
	void performProcCall(const std::string& name);
	void popStack();
	void pushStack(Procedure& proc);
	void performHalt();
	/**
     * Selects suitable method to perform instruction.
     * @param instruction to select
     */
	void selectInstruction(const std::string& instruction);
	/**
     * Checks if there is a wall or end of world in front of the robot
     * @param fillPosition is set with position that robot is looking at
     * @return true if there is a wall or end of the world, false otherwise
     */
	bool checkIfWallNextToRobot(Position& fillPosition);

public:
	/**
* Parses input files and initializes the interpret.
*
* \param   world       stream with the world definition
* \param   program     stream with the program source definition
*
* \note    Calls Complain::invalidSource, Complain::invalidWorld
*          or Complain::undefinedReference on errors in the source.
* \note    After successful initialization the interpret shall
*          be in the \a running state with the \c MAIN as the
*          active procedure.
*/
	Interpret(std::istream& world, std::istream& program)
	    : inWorld(world)
	    , inProgram(program) {
		world.peek();
		if (world.eof() || world.fail()) {
			Complain::invalidWorld(1, "world input stream is empty");
		}

		program.peek();
		if (program.eof() || program.fail()) {
			Complain::invalidSource(1, "program input stream is empty");
		}

		ParserWorld parserWorld(inWorld);
		_world = parserWorld.parseWorld(); // Parse world
		ParserProgram parserProgram(inProgram);
		_program = parserProgram.parseProgram(); // Parse program
		for (auto& i : _program.procedures) { // Find MAIN procedure and push it into stack
			if (i.name == "MAIN") {
				procedureStack.push(i.instructions.begin());
				procedureStackNames.push(i.name);
				break;
			}
		}
		isInterRunning = true;
	}

	/**
     * Returns a constant reference to the Robot.
     * The robot changes as the program is being interpreted.
     */
	const Robot& robot() const;

	/**
     * Returns a constant reference to the World.
     * The world changes as the program is being interpreted.
     */
	const World& world() const;

	/**
     * Returns \c true if there exists a procedure named \p name.
     *
     * \param   name        name of the procedure to search for
     */
	bool hasProcedure(const std::string& name) const;

	/**
     * Returns \c true if the interpret is in the \a running state.
     */
	bool running() const;

	/**
     * Takes a single step in the program execution.
     * Does nothing if the interpret is in the \a stopped state.
     *
     * \returns \c true if the interpret remains in the \a running state.
     */
	bool step();

	/**
     * Executes all steps of the program until it ends.
     * Does nothing if the interpret is in the \a stopped state.
     *
     * \returns the number of steps the interpret took to finish the program
     */
	unsigned run();
};

#endif //KAREL_INTERPRET_HPP
