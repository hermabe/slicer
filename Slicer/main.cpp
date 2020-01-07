#include <iostream>
#include <set>

#include "geometry.h"
#include "Object3D.h"
#include "graphics.h"

using std::cout;
using std::cerr;
using std::endl;

int main() {
	Object3D obj;
	obj.fromFile("stl/ring_txt.stl");
	Plane plane(Vector3d(0.0f, 0.0f, 5.0f), Vector3d(0.0f, 0.0f, 1.0f), Vector3d(1.0f, 0.0f, 0.0f));
	try
	{
		Polygon pol = obj.intersect(plane);

		saveAsSVG(pol, "pol.svg", 10);
		system("pol.svg");
	}
	catch (const std::exception & e)
	{
		cerr << e.what() << endl;
	}

	return 0;
}
