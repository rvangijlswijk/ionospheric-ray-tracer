//============================================================================
// Name        : SceneManager.cpp
// Author      : Rian van Gijlswijk
// Description : SceneManager
//============================================================================

#include <iostream>
#include <typeinfo>
#include <string>
#include "SceneManager.h"
#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace std;

	SceneManager::SceneManager() {

	}

	/**
	 * Find which object in the scene intersects with a ray
	 */
	Intersection SceneManager::intersect(Ray &r, Line2d &rayLine) {

		Vector2d pos,
				 rayOrigin;
		Intersection hit;
		hit.g.type = Geometry::none;
		rayOrigin.x = r.o.x;
		rayOrigin.y = r.o.y;

		for(Geometry g : sceneObjects) {

			pos = rayLine.intersect(g.getMesh());
			double smallestY = rayLine.begin.y;
			double biggestY = rayLine.end.y;
			if (rayLine.end.y < rayLine.begin.y) {
				smallestY = rayLine.end.y;
				biggestY = rayLine.begin.y;
			}
			double smallestX = rayLine.begin.x;
			double biggestX = rayLine.end.x;
			if (rayLine.end.x < rayLine.begin.x) {
				smallestX = rayLine.end.x;
				biggestX = rayLine.begin.x;
			}

			// is it within the scene and within the limits of the ray itself?
			if (smallestY < pos.y && pos.y < biggestY && smallestX < pos.x && pos.x < biggestX) {
				printf("boundaries: %8.4f %8.4f %8.4f %8.4f \n", smallestX, biggestX, smallestY, biggestY);
				printf("intersection: %8.4f %8.4f \n", pos.x, pos.y);
				hit.pos = pos;
				hit.o = g.type;
				hit.g = g;
				return hit;
			}
		}

		return hit;
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
