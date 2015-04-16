//============================================================================
// Name        : SceneManager.cpp
// Author      : Rian van Gijlswijk
// Description : SceneManager
//============================================================================

#include <iostream>
#include <typeinfo>
#include <string>
#include <list>
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
	Intersection SceneManager::intersect(Ray * r, Line3d & rayLine) {

		Vector3d pos;
		Intersection finalHit;
		finalHit.o = GeometryType::none;
		list<Intersection> hits;

		for (Geometry* gp : sceneObjects) {
			Geometry g = *gp;
			pos = rayLine.intersect(g.getMesh());

			double epsilon = 1e-5;
			if (abs(pos.x) > epsilon || abs(pos.y) > epsilon || abs(pos.z) > epsilon) {
				Intersection hit = Intersection();
				hit.pos = pos;
				hit.o = g.type;
				hit.g = gp;
				hits.push_back(hit);
			}
		}

		if (hits.size() > 0) {
			// evaluate which hit is closest
			double distance = 1e9;
			for (Intersection i : hits) {
				if (r->o.distance(i.pos) < distance && r->lastHit != i.g) {
					finalHit = i;
					distance = r->o.distance(i.pos);
				}
			}
		}

		return finalHit;
	}

	/**
	 * Add an object to the scene
	 */
	void SceneManager::addToScene(Geometry* obj) {

		sceneObjects.push_back(obj);
	}

	/**
	 * Return a list of all objects in the scene
	 */
	list<Geometry*> SceneManager::getScene() {

		return sceneObjects;
	}

	/**
	 * Remove all objects currently defined in the scene
	 */
	void SceneManager::removeAllFromScene() {

		sceneObjects.clear();
	}

} /* namespace scene */
} /* namespace raytracer */
