#pragma once
#include <iostream>
#include <map>
#include <string>

#include "json.hpp"

#include "geometry.h"


using json = nlohmann::json;

struct Machine
{
	static Machine fromFile(const std::string& filename);

	friend void to_json(json& j, const Machine& m);
	friend void from_json(const json& j, Machine& m);

	float bedX, bedY, bedZ;
	float headXmin, headXmax, headYmin, headYmax;
	float gantryHeight;
	bool originAtCenter = false;
	bool heatedBed = false;
	std::string startGcode = "", endGcode = "";
	std::string name;
};

enum class Adhesion { None, Skirt, Brim, Raft };

struct PrintSettings {
	static PrintSettings fromFile(const std::string& filename);

	friend void to_json(json& j, const PrintSettings& s);
	friend void from_json(const json& j, PrintSettings& s);

	Vector2d position = { 0, 0, 0 };
	Vector3d direction = { 0, 0, 1 };
	Vector3d tangent = { 1, 0, 0 };
	float layerheight = 0.1f;
	float infill = 0.1f;
	float scale = 1.0f;
	Adhesion adhesion = Adhesion::None;
};
