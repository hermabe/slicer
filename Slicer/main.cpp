#include <iomanip>
#include <iostream>
#include <sstream>

#include "geometry.h"
#include "Object3D.h"
#include "graphics.h"

using std::cout;
using std::cerr;
using std::endl;

int main() {
	Object3D obj;
	obj.fromFile("stl/u_bin.stl", true);
	Plane plane(Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 0.0f, 1.0f), Vector3d(1.0f, 0.0f, 0.0f));
	//Polygon pol = obj.intersect(plane);
	std::vector<Polygon> slices = obj.slice(plane, 1.0f);
	for (int i = 0; i < slices.size(); ++i) {
		std::stringstream ss;
		ss << std::setw(4) << std::setfill('0') << i;
		saveAsSVG(slices[i], "svg/" + ss.str() + ".svg", 1);
	}

	return 0;
}
