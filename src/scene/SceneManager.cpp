//============================================================================
// Name        : SceneManager.cpp
// Author      : Rian van Gijlswijk
// Description : SceneManager
//============================================================================

#include <iostream>
#include "SceneManager.h"

namespace raytracer {
namespace scene {

	using namespace std;

	// ionosphere
	Line2f ionosphere;
	// terrain
	Line2f terrain;

	SceneManager::SceneManager() {

		// init scene
		ionosphere.begin = Vector2f(0, 100);
		ionosphere.end = Vector2f(100, 100);
		terrain.begin = Vector2f(0, 1);
		terrain.end = Vector2f(100, 1);
	}

	Intersection SceneManager::intersect(Ray &r, Line2f &rayLine) {

		Vector2f pos;
		Intersection is;
		Vector2f rayOrigin;
		rayOrigin.x = r.o.x;
		rayOrigin.y = r.o.y;
		pos = rayLine.intersect(ionosphere);
		// cout << "intersection:(" << pos.x << "," << pos.y << ")\n";

		// is it within the scene?
		if (pos.y > 0 && pos.y <= 150 && pos.x > 0 && pos.x < 300 && pos.distance(rayOrigin) > 1) {
			is.pos = pos;
			is.o = Intersection::ionosphere;
			cout << "Hit ionosphere!\n";
		} else {
			pos = rayLine.intersect(terrain);
			cout << "intersection:(" << pos.x << "," << pos.y << ")\n";
			if (pos.y > 0 && pos.y <= 150 && pos.x > 0 && pos.x < 300 && pos.distance(rayOrigin) > 1) {
				is.pos = pos;
				is.o = Intersection::terrain;
				cout << "Hit terrain!\n";
			} else {
				is.o = Intersection::none;
				cout << "Hit none!\n";
			}
		}

		return is;
	}

} /* namespace scene */
} /* namespace raytracer */
