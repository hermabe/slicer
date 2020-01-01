#pragma once
#include <vector>

using std::istream;
using std::string;
using std::vector;


class Vector3d {
public:
	Vector3d(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3d() : Vector3d(0, 0, 0) {}
private:
	float x, y, z;
};

class Triangle {
public:
	Vector3d normal;
	Vector3d vertices[3];
};

class Object3D
{
public:
private:
	vector<Triangle> triangles;
};

