#include <iostream>

#include "Object3D.h"

using namespace std;

int main() {
	Object3D obj_bin;
	try {
		//obj.fromFile("Sphericon.stl");
		obj_bin.fromFile("Sphericon_bin.stl", true);
	}
	catch (exception & e) {
		cout << e.what() << endl;
	}

	Object3D obj_txt;
	try {
		//obj.fromFile("Sphericon.stl");
		obj_txt.fromFile("Sphericon_txt.stl", false);
	}
	catch (exception & e) {
		cout << e.what() << endl;
	}

	return 0;
}
