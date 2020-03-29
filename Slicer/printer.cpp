#include <algorithm>
#include <fstream>
#include <iomanip>
#include <unordered_map>

#include "printer.h"

Machine Machine::fromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open file " + filename);
	}
	json j;
	try
	{
		file >> j;
	}
	catch (const std::exception& e)
	{
		std::string what = e.what();
		throw std::runtime_error("JSON ERROR: " + what);
	}

	return j.get<Machine>();
}

void to_json(json& j, const Machine& m) {
	j = json{
		{"name", m.name},
		{"bedX", m.bedX},
		{"bedY", m.bedY},
		{"bedZ", m.bedZ},
		{"headXmin", m.headXmin},
		{"headXmax", m.headXmax},
		{"headYmin", m.headYmin},
		{"headYmax", m.headYmax},
		{"gantryHeight", m.gantryHeight},
		{"originAtCenter", m.originAtCenter},
		{"heatedBed", m.heatedBed},
		{"startGcode", m.startGcode},
		{"endGcode", m.endGcode},
	};
}

void from_json(const json& j, Machine& m) {
	j.at("name").get_to(m.name);
	j.at("bedX").get_to(m.bedX);
	j.at("bedY").get_to(m.bedY);
	j.at("bedZ").get_to(m.bedZ);
	j.at("headXmin").get_to(m.headXmin);
	j.at("headXmax").get_to(m.headXmax);
	j.at("headYmin").get_to(m.headYmin);
	j.at("headYmax").get_to(m.headYmax);
	j.at("gantryHeight").get_to(m.gantryHeight);
	j.at("originAtCenter").get_to(m.originAtCenter);
	j.at("heatedBed").get_to(m.heatedBed);
	j.at("startGcode").get_to(m.startGcode);
	j.at("endGcode").get_to(m.endGcode);
}

PrintSettings PrintSettings::fromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open file " + filename);
	}
	json j;
	try
	{
		file >> j;
	}
	catch (const std::exception& e)
	{
		std::string what = e.what();
		throw std::runtime_error("JSON ERROR: " + what);
	}

	return j.get<PrintSettings>();
}

std::string to_string(Adhesion a) {
	switch (a)
	{
	case Adhesion::None:
		return "None";
	case Adhesion::Skirt:
		return "Skirt";
	case Adhesion::Brim:
		return "Brim";
	case Adhesion::Raft:
		return "Raft";
	}
}


Adhesion from_string(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), [](auto c) {return std::tolower(c); });
	static const std::unordered_map<std::string, Adhesion> map{
		{"none", Adhesion::None },
		{"skirt", Adhesion::Skirt },
		{"brim", Adhesion::Brim },
		{"raft", Adhesion::Raft }
	};
	return map.at(s);
}

void to_json(json& j, const PrintSettings& s) {
	j = json{
		{"layerheight", s.layerheight},
		{"infill", s.infill},
		{"scale", s.scale},
		{"adhesion", to_string(s.adhesion)},
		{"position", {s.position[0], s.position[1]}},
		{"direction", {s.direction[0], s.direction[1], s.direction[2]}},
		{"tangent", {s.tangent[0], s.tangent[1], s.tangent[2]}},
	};
}

void from_json(const json& j, PrintSettings& s) {
	j.at("layerheight").get_to(s.layerheight);
	j.at("infill").get_to(s.infill);
	j.at("scale").get_to(s.scale);
	std::string adhesionString;
	j.at("adhesion").get_to(adhesionString);
	s.adhesion = from_string(adhesionString);
}
