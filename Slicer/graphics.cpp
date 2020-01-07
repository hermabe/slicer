#include <fstream>
#include <string>
#include <sstream>

#include "graphics.h"

std::string toSVG(const SimplePolygon& poly, float scale) {
	std::stringstream ss;
	ss << "M " << poly.vertices[0][0] * scale << " " << poly.vertices[0][1] * scale << " ";
	for (unsigned int i = 1; i < poly.vertices.size(); ++i) {
		ss << "L " << poly.vertices[i][0] * scale << " " << poly.vertices[i][1] * scale << " ";
	}
	ss << "Z ";
	return ss.str();
}

void saveAsSVG(const Polygon& poly, const std::string& filename, float scale)
{
	std::ofstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open file");
	}
	file << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
	file << "height=\"100%\" width=\"100%\" style=\"fill: darkgrey; stroke: black; stroke-width: 2;\">" << std::endl;
	file << "<path fill-rule=\"evenodd\" d=\"";
	file << toSVG(poly.exterior, scale) << std::endl;
	for (const auto& interior : poly.interior) {
		file << toSVG(interior, scale) << std::endl;
	}
	file << "\"/>" << std::endl;
	file << "</svg>";
}
