#include <fstream>

#include "gcode.h"


Command Command::rapid(float F, float X, float Y) {
	return Command{ 'G', 0, {{'F', F}, {'X', X}, {'Y', Y}} };
}

Command Command::move(float E, float F, float X, float Y) {
	return Command{ 'G', 1, {{'E', E}, {'F', F}, {'X', X}, {'Y', Y}} };
}

Command Command::retract() {
	return Command{ 'G', 10, {} };
}

Command Command::unRetract() {
	return Command{ 'G', 11, {} };
}

Command Command::resetCoordinate(char coord, float value) {
	return Command{ 'G', 92, {{coord, value}} };
}

Command Command::extrusionAbsolute() {
	return Command{ 'G', 82, {} };
}

Command Command::fanOn(float S) {
	return Command{ 'M', 106, {{'S', S}} };
}

Command Command::fanOff() {
	return Command{ 'M', 107, {} };
}


std::ostream& operator<<(std::ostream& out, const Command& c)
{
	out << c.type << c.number;
	for (const auto& [parameter, value] : c.parameters) {
		out << " " << parameter << value;
	}
	return out;
}

std::vector<Command> generateGcode(const std::vector<Polygon>& layers, const Machine& machine, const PrintSettings& settings)
{
	return std::vector<Command>();
}

void toFile(const std::vector<Command>& commands, const std::string& filename)
{
	std::ofstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open file " + filename);
	}
	for (const auto& command : commands) {
		file << command << '\n';
	}
}
