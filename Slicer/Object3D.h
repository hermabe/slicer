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
	std::vector<Polygon> slice(const Plane& start, float layerHeight);
private:
	std::vector<Triangle3d> triangles;

	void fromASCIIFile(string filename);
	void fromBinaryFile(string filename);
	std::pair<float, float> minMax(const Vector3d& direction);
};
