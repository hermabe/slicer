#pragma once
#include <array>
#include <vector>

#include "vector.h"

constexpr float PI = 3.14159265359f;


template<typename T, unsigned int N>
class Triangle
{
public:
	Vector<T, N> normal;
	std::array<Vector<T, N>, 3> vertices;
};

typedef Triangle<float, 3> Triangle3d;
typedef Triangle<float, 2> Triangle2d;

template<typename T, unsigned int N>
struct Edge {
	Vector<T, N> start, end, normal;
};

typedef Edge<float, 3> Edge3d;
typedef Edge<float, 2> Edge2d;

class Plane {
public:
	Vector3d point, normal, xAxis;

	Plane(const Vector3d& point, const Vector3d& normal, const Vector3d& xAxis)
		: point(point), normal(normal.normalized()), xAxis(xAxis.normalized()) {};
	Vector3d yAxis() const {
		return cross(normal, xAxis).normalized();
	}
};

class Line {
public:
	Vector3d point, direction;

	Line(const Edge3d& edge) : point(edge.start), direction(edge.end - edge.start) {};
	Line(const Vector3d& point, const Vector3d& direction) : point(point), direction(direction) {};
};

struct SimplePolygon {
	// Represents a simply connected polygon
	std::vector<Vector2d> vertices;
};

struct Polygon {
	// Represents a complex polygon.
	// Can have zero or more "holes" which are SimplePolygons
	std::vector<SimplePolygon> paths;
};

std::vector<Triangle3d> intersects(const std::vector<Triangle3d>& triangles, const Plane& plane);
Vector3d intersection(const Plane& plane, const Line& line);
std::vector<Edge3d> intersection(const Plane& plane, const Triangle3d& triangle);

Vector2d project(const Plane& plane, const Vector3d& vector);
Edge2d project(const Plane& plane, const Edge3d& edge);

std::vector<Triangle3d> withVertex(const std::vector<Triangle3d>& triangles, const Vector3d& vertex);
Triangle3d withEdge(const std::vector<Triangle3d>& triangles, const Vector3d& firstVertex, const Vector3d& secondVertex);
Triangle3d withEdge(const std::vector<Triangle3d>& withVertex, const Vector3d& secondVertex);

float directionalAngle(const Vector2d& lhs, const Vector2d& rhs);
float saturate(float value, float minimum, float maximum);
