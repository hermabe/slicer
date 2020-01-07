#pragma once
#include <string>
#include <vector>

#include "geometry.h"

using std::string;

class Object3D
{
public:
	void fromFile(string filename, bool binary = false);
	Polygon intersect(const Plane& plane) const;
private:
	std::vector<Triangle3d> triangles;

	void fromASCIIFile(string filename);
	void fromBinaryFile(string filename);
};
