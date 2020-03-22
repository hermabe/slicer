#include <fstream>
#include <list>
#include <optional>
#include <regex>
#include <sstream>

#include "Object3D.h"

using std::ifstream;
using std::regex;
using std::stringstream;

float bytesToFloat(unsigned char* n) {
	float* f = reinterpret_cast<float*>(n);
	return *f;
}

Vector3d bytesToVector3d(unsigned char bytes[12]) {
	return Vector3d(bytesToFloat(bytes), bytesToFloat(bytes + 4), bytesToFloat(bytes + 8));
}

Triangle3d readBinaryFacet(ifstream& file) {
	Triangle3d triangle;

	// Read normal 3*4 byte little-endian float
	constexpr int buf_size = 12;
	unsigned char buffer[buf_size];
	file.read((char*)buffer, buf_size);
	triangle.normal = bytesToVector3d(buffer);

	// Read 3 vectors 3*3*4 byte little-endian float
	for (unsigned int i = 0; i < 3; i++)
	{
		file.read((char*)buffer, buf_size);
		triangle.vertices[i] = bytesToVector3d(buffer);
	}

	// Read attribute count 2 byte. Should be zero
	char attribute_size[2];
	file.read(attribute_size, 2);
	//if (attribute_size[0] | attribute_size[1]) {
	//	std::cerr << (unsigned int)attribute_size[0] << " " << (unsigned int)attribute_size[1] << std::endl;
	//	//throw std::runtime_error("Attribute count must be zero");
	//}

	return triangle;
}

void Object3D::fromBinaryFile(string filename) {
	ifstream file(filename, std::ios::binary | std::ios::in);
	if (!file) {
		throw std::runtime_error("Could not open file " + filename);
	}

	// Read header and ignore
	char header[80];
	file.read(header, 80);

	// Read size, little endian
	unsigned char size_bytes[4];
	file.read((char*)size_bytes, 4);

	unsigned int size = size_bytes[3] * 256 * 256 * 256 + size_bytes[2] * 256 * 256 + size_bytes[1] * 256 + size_bytes[0];

	// Read size number of triangles
	for (unsigned int i = 0; i < size; i++)
	{
		this->triangles.push_back(readBinaryFacet(file));
	}

	//// Should be no more data
	//if (file) {
	//	vector<unsigned char> data;
	//	char byte[1];
	//	while (file.read(byte, 1)) {
	//		data.push_back(byte[0]);
	//	}
	//	throw std::runtime_error("Size mismatch");
	//}
}

Triangle3d readASCIIFacet(ifstream& file) {
	regex normal("^ *normal .*");
	regex loopstart("^ *outer loop$");
	regex vertexstart("^ *vertex .*");
	regex loopend("^ *endloop$");
	regex facetend("^ *endfacet$");

	string line;

	// Read facet start
	if (!getline(file, line)) {
		throw std::runtime_error("Facet normal not found");
	}
	if (!regex_match(line.begin(), line.end(), normal)) {
		throw std::runtime_error("Expected facet normal");
	}
	string junk;
	stringstream ss(line);
	ss >> junk; // Remove "normal"
	Triangle3d triangle;
	ss >> triangle.normal;

	// Read loop start
	if (!getline(file, line)) {
		throw std::runtime_error("Loop not found");
	}
	if (!regex_match(line.begin(), line.end(), loopstart)) {
		throw std::runtime_error("Expected loop");
	}

	// Read vertices
	for (int i = 0; i < 3; i++)
	{
		if (!getline(file, line)) {
			throw std::runtime_error("Vertex not found");
		}
		if (!regex_match(line.begin(), line.end(), vertexstart)) {
			throw std::runtime_error("Expected vertex");
		}
		ss = stringstream(line);
		ss >> junk; // Remove "vertex"
		ss >> triangle.vertices[i];
	}

	// Read loop end
	if (!getline(file, line)) {
		throw std::runtime_error("Loop end not found");
	}
	if (!regex_match(line.begin(), line.end(), loopend)) {
		throw std::runtime_error("Expected loop end");
	}

	// Read facet end
	if (!getline(file, line)) {
		throw std::runtime_error("Facet end not found");
	}
	if (!regex_match(line.begin(), line.end(), facetend)) {
		throw std::runtime_error("Expected facet end");
	}

	return triangle;
}

void Object3D::fromASCIIFile(string filename) {
	regex solidstart("^solid \\S*");
	regex solidend("endsolid");
	regex facetstart("facet");

	ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open file " + filename);
	}

	string line;
	if (!getline(file, line))
	{
		throw std::runtime_error("File does not include a solid");
	}

	if (!regex_match(line.begin(), line.end(), solidstart))
	{
		throw std::runtime_error("First line must start with \"solid\".");
	}


	while (true) {
		file >> line;
		if (regex_match(line.begin(), line.end(), facetstart)) {
			this->triangles.push_back(readASCIIFacet(file));
		}
		else if (regex_match(line.begin(), line.end(), solidend)) {
			break;
		}
		else {
			throw std::runtime_error("Invalid syntax");
		}
	}
}

void Object3D::fromFile(string filename, bool binary)
{
	regex fileExtension(".+\\.stl$");
	if (!regex_match(filename.begin(), filename.end(), fileExtension)) {
		throw std::runtime_error("Filetype must be stl");
	}

	if (binary)
	{
		fromBinaryFile(filename);
	}
	else
	{
		fromASCIIFile(filename);
	}

}

Line get_triangle_edge(const Triangle3d& triangle, unsigned int index)
{
	return Line{ triangle.vertices[index], (triangle.vertices[(index + 1) % 3] - triangle.vertices[index]) };
}

std::optional<std::pair<float, Vector2d>>  angleAndVertexIfClose(const Edge2d& edge, const Vector2d& vertex, const Vector2d& direction) {
	if (vertex.isClose(edge.start)) {
		return std::make_pair(directionalAngle(direction, edge.end - edge.start), edge.end);
	}
	else if (vertex.isClose(edge.end)) {
		return std::make_pair(directionalAngle(direction, edge.start - edge.end), edge.start);
	}
	return {};
}

std::optional<Vector2d> extractClose(std::list<Edge2d>& edges, const Vector2d& vertex, const Vector2d& direction) {
	Vector2d minVertex;
	auto minIt = edges.end();
	float minAngle = INFINITY;
	for (auto it = edges.begin(), end = edges.end(); it != end; ++it) {
		auto angleAndVertex = angleAndVertexIfClose(*it, vertex, direction);
		if (!angleAndVertex) {
			// Edge not close, skip
			continue;
		}

		auto [angle, vertex] = *angleAndVertex;

		// Find minimum
		if (angle < minAngle) {
			minVertex = vertex;
			minIt = it;
			minAngle = angle;
		}
	}

	if (minIt != edges.end())
	{
		edges.erase(minIt);
		return minVertex;
	}
	return {};
}

std::optional<SimplePolygon> linkEdges(std::list<Edge2d>& edges) {
	// Removes elements from edges and combines them into a simple polygon
	auto max = std::max_element(edges.begin(), edges.end(),
		[](const Edge2d& lhs, const Edge2d& rhs) {
			// Find edge furthest in one direction to ensure to be on outermost perimeter
			return std::max(lhs.start[0], lhs.end[0]) < std::max(rhs.start[0], rhs.end[0]);
		});

	SimplePolygon polygon;
	Vector2d start = max->start[0] > max->end[0] ? max->start : max->end;
	Vector2d vertex = start, prev;
	Vector2d direction(1.0f, 0.0f);
	do {
		polygon.vertices.push_back(vertex);

		auto extraction = extractClose(edges, vertex, direction);
		if (!extraction) {
			return {};
		}
		prev = vertex;
		vertex = *extraction;
		direction = vertex - prev;
	} while (!vertex.isClose(start));
	return polygon;
}

Polygon Object3D::intersect(const Plane& plane) const
{
	std::vector<Triangle3d> intersectingTriangles = intersects(this->triangles, plane);
	if (intersectingTriangles.empty()) {
		throw std::runtime_error("Plane does not intersect");
	}

	// Find intersections between plane and triangles.
	std::list<Edge3d> edges3d;
	for (const Triangle3d& triangle : intersectingTriangles)
	{
		std::vector<Edge3d> edges = intersection(plane, triangle);
		edges3d.insert(edges3d.end(), edges.cbegin(), edges.cend());
	}

	// Project edges into plane
	std::list<Edge2d> projected(edges3d.size());
	std::transform(edges3d.cbegin(), edges3d.cend(), projected.begin(),
		[&plane](const Edge3d edge) {
			return project(plane, edge);
		});

	// Remove edges from triangles in plane
	projected.erase(
		std::remove_if(projected.begin(), projected.end(), [](const Edge2d& edge) {
			return edge.normal.length() < FLOATERROR;
			}),
		projected.end());

	// Link edges
	Polygon intersection;
	auto poly = linkEdges(projected);
	if (!poly) {
		throw std::runtime_error("Could not link edges");
	}
	intersection.paths.push_back(*poly);
	while (!projected.empty()) {
		poly = linkEdges(projected);
		if (!poly) {
			break;
		}
		intersection.paths.push_back(*poly);
	}

	return intersection;
}

Polygon combineEdges(Polygon poly) {
	for (auto& path : poly.paths) {
		for (unsigned int i = 0; i < path.vertices.size(); i++) {
			unsigned int size = path.vertices.size();
			Vector2d prev = path.vertices[i] - path.vertices[(i + size - 1) % size];
			Vector2d next = path.vertices[(i + 1 + size) % size] - path.vertices[i];
			float cross = prev[0] * next[1] - prev[1] * next[0]; // 2D cross product
			if (abs(cross) < FLOATERROR) {
				// Remove, edge from prev to next is a straight line
				path.vertices.erase(path.vertices.begin() + i);
			}
		}
	}
	return poly;
}

std::vector<Polygon> Object3D::slice(const Plane& plane, float layerHeight)
{
	std::vector<Polygon> slices;
	Plane intersector = plane;
	auto [min, max] = minMax(plane.normal);
	float startDistance = plane.point.dot(intersector.normal) / intersector.normal.length();

	for (float distance = min; distance <= max; distance += layerHeight) {
		intersector.point = plane.point + plane.normal * (distance - startDistance);
		slices.push_back(combineEdges(intersect(intersector)));
	}

	return slices;
}

std::pair<float, float> Object3D::minMax(const Vector3d& direction)
{
	float min = INFINITY, max = -INFINITY;
	for (const auto& triangle : this->triangles)
	{
		for (const auto& vertex : triangle.vertices) {
			float distance = vertex.dot(direction);
			if (distance < min) {
				min = distance;
			}
			if (distance > max) {
				max = distance;
			}
		}
	}
	// Length of direction factored out from loop
	float directionLength = direction.length();
	return std::pair<float, float>(min / directionLength, max / directionLength);
}
