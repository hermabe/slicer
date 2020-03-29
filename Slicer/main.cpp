#include <iomanip>
#include <iostream>
#include <sstream>

#include <fstream>

#include "gcode.h"
#include "geometry.h"
#include "graphics.h"
#include "Object3D.h"
#include "printer.h"

#include "json.hpp"

using std::cout;
using std::cerr;
using std::endl;

int main() {
	PrintSettings s = PrintSettings::fromFile("settings/settings.json");
	Machine m = Machine::fromFile("settings/UM2.json");
	Command c = Command::fanOff();
	
	//Object3D obj;
	//string filename = "cube_bin";
	//obj.fromFile("stl/" + filename +".stl", true);
	//
	//Plane plane(Vector3d(s.position), s.direction, s.tangent);
	//std::vector<Polygon> slices = obj.slice(plane, s.layerheight);
	//for (int i = 0; i < slices.size(); ++i) {
	//	cout << i << ": ";
	//	for (int j = 0; j < slices[i].paths.size(); j++) {
	//		cout << slices[i].paths[j].vertices.size() << " ";
	//	}
	//	cout << endl;
	//	std::stringstream ss;
	//	ss << std::setw(4) << std::setfill('0') << i;
	//	saveAsSVG(slices[i], "svg/" + ss.str() + ".svg", 1);
	//}
	//
	//toFile(generateGcode(slices, m, s), "gcode/" + filename + ".gcode");

	
	
	return 0;
}
