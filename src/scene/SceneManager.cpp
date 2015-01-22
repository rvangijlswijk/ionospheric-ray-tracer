//============================================================================
// Name        : SceneManager.cpp
// Author      : Rian van Gijlswijk
// Description : SceneManager
//============================================================================

#include <iostream>
#include <typeinfo>
#include <string>
#include "SceneManager.h"

namespace raytracer {
namespace scene {

	using namespace std;

	SceneManager::SceneManager() {

	}

	/**
	 * Find which object in the scene intersects with a ray
	 */
	Intersection SceneManager::intersect(Ray &r, Line2f &rayLine) {

		Vector2f pos;
		Intersection is;
		Vector2f rayOrigin;
		rayOrigin.x = r.o.x;
		rayOrigin.y = r.o.y;

		for(Geometry g : sceneObjects) {

			pos = rayLine.intersect(g.getMesh());
			// is it within the scene?
			if (pos.y > 0 && pos.y <= 150 && pos.x > 0 && pos.x < 300 && pos.distance(rayOrigin) > 1) {
				cout << "intersection:(" << pos.x << "," << pos.y << ")\n";
				is.pos = pos;
				is.o = g.type;
				is.g = g;
				return is;
			}
		}

		return is;
	}

	/**
	 * Add an object to the scene
	 */
	void SceneManager::addToScene(Geometry obj) {

		sceneObjects.push_back(obj);
	}

	/**
	 * Return a list of all objects in the scene
	 */
	list<Geometry> SceneManager::getScene() {

		return sceneObjects;
	}

} /* namespace scene */
} /* namespace raytracer */
