#include <algorithm>
#include <array>
#include <cassert>
#include <functional>

#include "geometry.h"

Vector3d intersection(const Plane& plane, const Line& line)
{
	// Assumes line intersects plane at a single point
	float distance = (plane.point - line.point).dot(plane.normal) / (line.direction.dot(plane.normal));
	return line.point + line.direction * distance;
}

float planeCosine(const Plane& plane, const Vector3d& vertex) {
	// Cosine of angle between vertex and plane normal
	return plane.normal.dot(vertex - plane.point);
};

std::array<int, 3> vertexPositions(const Plane& plane, const Triangle3d& triangle) {
	std::array<int, 3> result;
	std::transform(triangle.vertices.cbegin(), triangle.vertices.cend(), result.begin(),
		[&plane](const Vector3d& vertex) {
			float cosine = planeCosine(plane, vertex);
			if (cosine > FLOATERROR)
			{
				return 1;
			}
			else if (cosine < -FLOATERROR) {
				return -1;
			}
			else {
				return 0;
			}
		});
	return result;
}

std::array<Edge3d, 2> getIntersectingEdges(const std::array<int, 3>& positions, const Triangle3d& triangle) {
	// Assumes triangle intersects plane
	std::array<Edge3d, 2> edges;
	unsigned int count = 0;
	if (positions[0] && (positions[0] * -1 == positions[1])) {
		edges[count++] = Edge3d{ triangle.vertices[0], triangle.vertices[1] , triangle.normal };
	}
	if (positions[0] && (positions[0] * -1 == positions[2])) {
		edges[count++] = Edge3d{ triangle.vertices[2], triangle.vertices[0] , triangle.normal };
	}
	if (positions[1] && (positions[1] * -1 == positions[2])) {
		edges[count++] = Edge3d{ triangle.vertices[1], triangle.vertices[2], triangle.normal };
	}
	assert(count == 2); // Should be 2 edges
	return edges;
}

std::vector<Edge3d> intersection(const Plane& plane, const Triangle3d& triangle)
{
	// Assumes triangle intersects plane
	std::array<int, 3> position = vertexPositions(plane, triangle);
	long long int numberOfVerticesInPlane = std::count_if(position.cbegin(), position.cend(),
		[](int value) {
			return value == 0;
		});

	auto indexOfInPlane = [](std::array<int, 3>& arr) {
		for (unsigned int i = 0; i < 3; i++)
		{
			if (arr[i] == 0) {
				return i;
			}
		}
		assert(false); // At least one edge should be in plane
	};

	std::vector<Edge3d> edges;

	switch (numberOfVerticesInPlane)
	{
	case 0: {
		// Find two intersection points, make edge
		std::array<Edge3d, 2> intersectingEdges = getIntersectingEdges(position, triangle);
		Vector3d start = intersection(plane, Line(intersectingEdges[0]));
		Vector3d end = intersection(plane, Line(intersectingEdges[1]));
		edges.emplace_back(Edge3d{ start, end, triangle.normal });
		break;
	}
	case 1: {
		unsigned int index = indexOfInPlane(position);
		// Check if rest of triangle intersects
		if (position[0] + position[1] + position[2] == 0) {
			// Other two vertices are on different side of plane, find intersection
			Edge3d opposite = Edge3d{ triangle.vertices[(index + 1) % 3], triangle.vertices[(index + 2) % 3], triangle.normal };
			Vector3d intersect = intersection(plane, Line(opposite));
			edges.emplace_back(Edge3d{ triangle.vertices[index], intersect, triangle.normal });
		}
		break;
	}
	case 2: {
		// Make edge with start, end = two vertices in plane
		unsigned int start = indexOfInPlane(position);
		position[start] = 2; // Do not choose same edge twice
		unsigned int end = indexOfInPlane(position);
		edges.emplace_back(Edge3d{ triangle.vertices[start], triangle.vertices[end], triangle.normal });
		break;
	}
	case 3: {
		// Make three edges, one for each edge in triangle
		for (unsigned int i = 0; i < 3; i++)
		{
			edges.emplace_back(Edge3d{ triangle.vertices[i], triangle.vertices[(i + 1) % 3] });
		}
		break;
	}
	default:
		throw std::logic_error("A triangle cannot have more than 3 vertices.");
	}
	return edges;
}

float project(const Vector3d& base, const Vector3d& vector) {
	return base.dot(vector) / base.length();
}

Vector2d project(const Plane& plane, const Vector3d& vector)
{
	Vector3d yAxis = plane.yAxis();
	Vector3d shifted = vector - plane.point;
	return Vector2d(project(plane.xAxis, shifted), project(yAxis, shifted));
}

Edge2d project(const Plane& plane, const Edge3d& edge)
{
	return Edge2d{ project(plane, edge.start), project(plane, edge.end), project(plane, edge.normal) };
}


std::vector<Triangle3d>withVertex(const std::vector<Triangle3d>& triangles, const Vector3d& vertex)
{
	std::vector<Triangle3d> result;
	std::copy_if(triangles.cbegin(), triangles.cend(), result.begin(), [&vertex](const Triangle3d& triangle) {
		return std::any_of(triangle.vertices.cbegin(), triangle.vertices.cend(),
			[&vertex](const Vector3d& triangle_vertex) {
				return vertex.isClose(triangle_vertex);
			});
		});
	return result;
}

Triangle3d withEdge(const std::vector<Triangle3d>& triangles, const Vector3d& firstVertex, const Vector3d& secondVertex)
{
	return withEdge(withVertex(triangles, firstVertex), secondVertex);
}

Triangle3d withEdge(const std::vector<Triangle3d>& trianglesWithVertex, const Vector3d& secondVertex)
{
	return withVertex(trianglesWithVertex, secondVertex)[0];
}

float directionalAngle(const Vector2d& lhs, const Vector2d& rhs)
{
	float angle = std::acosf(saturate(lhs.dot(rhs) / (lhs.length() * rhs.length()), -1.0f, 1.0f));
	float cross = lhs[0] * rhs[1] - lhs[1] * rhs[0];
	return cross > 0 ? angle : 2 * PI - angle;
}

float saturate(float value, float minimum, float maximum)
{
	return std::max(minimum, std::min(value, maximum));
}

bool aboveOrBelow(const Plane& plane, const Triangle3d& triangle) {
	// Returns true if all the vertices is on the same side of the plane
	// Returns false if triangle intersects plane

	auto above = [&plane](const Vector3d& vertex) { return planeCosine(plane, vertex) > 0; };
	auto below = [&plane](const Vector3d& vertex) { return planeCosine(plane, vertex) < 0; };

	return std::all_of(triangle.vertices.cbegin(), triangle.vertices.cend(), above)
		|| std::all_of(triangle.vertices.cbegin(), triangle.vertices.cend(), below);
}

std::vector<Triangle3d> intersects(const std::vector<Triangle3d>& triangles, const Plane& plane)
{
	std::vector<Triangle3d> result(triangles.size());
	auto it = std::copy_if(triangles.cbegin(), triangles.cend(), result.begin(),
		[&plane](const Triangle3d& triangle) {
			return !aboveOrBelow(plane, triangle);
		});
	result.resize(std::distance(result.begin(), it));
	return result;
}
