#pragma once
#include <string>
#include <vector>

#include "geometry.h"

using std::istream;
using std::string;
using std::vector;

class Object3D
{
public:
	void fromFile(string filename, bool binary = false);

private:
	vector<Triangle> triangles;

	void fromASCIIFile(string filename);
	void fromBinaryFile(string filename);
};
