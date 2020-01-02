#include "geometry.h"

Vector3d intersection(const Plane& plane, const Line& line)
{
	// Assumes line intersects plane at a single point
	float distance = (plane.point - line.point).dot(plane.normal) / (line.direction.dot(plane.normal));
	return line.point + line.direction * distance;
}
