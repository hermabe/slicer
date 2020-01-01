#include <iostream>

#include "geometry.h"
#include "Object3D.h"

using namespace std;

int main() {
	Vector3d v3;
	cout << v3 << endl;
	Vector2d v2;
	cout << v2 << endl;
	Vector<4> v4;
	cout << v4 << endl;
	v4[0] = 1.0f;
	cout << v4 << endl;
	cout << v4 + v4 << endl;
	cout << (v4*4).dot(v4*2) << endl;


	//Object3D obj_bin;
	//try {
	//	//obj.fromFile("Sphericon.stl");
	//	obj_bin.fromFile("Sphericon_bin.stl", true);
	//}
	//catch (exception & e) {
	//	cout << e.what() << endl;
	//}

	//Object3D obj_txt;
	//try {
	//	//obj.fromFile("Sphericon.stl");
	//	obj_txt.fromFile("Sphericon_txt.stl", false);
	//}
	//catch (exception & e) {
	//	cout << e.what() << endl;
	//}

	return 0;
}
