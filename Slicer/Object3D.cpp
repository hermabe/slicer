#include <cmath>
#include <fstream>
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

Triangle readBinaryFacet(ifstream& file) {
	Triangle triangle;

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

Triangle readASCIIFacet(ifstream & file) {
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
	Triangle triangle;
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
