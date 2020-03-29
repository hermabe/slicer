#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "geometry.h"
#include "printer.h"

class Command {
private:
	char type;
	unsigned int number;
	std::map<char, float> parameters;
	Command(char type, unsigned int number, const std::map<char, float>& parameters)
		: type{ type }, number{ number }, parameters{ parameters } {}
public:
	Command() = delete;
	static Command rapid(float F, float X, float Y);
	static Command move(float E, float F, float X, float Y);
	static Command retract();
	static Command unRetract();
	static Command resetCoordinate(char coord, float value);
	static Command extrusionAbsolute();
	static Command fanOn(float S);
	static Command fanOff();

	friend std::ostream& operator<<(std::ostream& os, const Command& command);
};


std::vector<Command> generateGcode(const std::vector<Polygon>& layers, const Machine& machine, const PrintSettings& settings);
void toFile(const std::vector<Command>& commands, const std::string& filename);
