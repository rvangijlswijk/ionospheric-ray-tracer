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
	Intersection SceneManager::intersect(Ray &r, Line2d &rayLine) {

		Vector2d pos,
				 rayOrigin;
		typedef std::list<Geometry*>::const_iterator geometryIter;
		list<Intersection> hits;
		//hit.g->type = Geometry::none;
		rayOrigin.x = r.o.x;
		rayOrigin.y = r.o.y;

//		for (geometryIter i = sceneObjects.begin(); i != sceneObjects.end(); ++i) {
		for (Geometry* gp : sceneObjects) {
//			Geometry &g = **i;
			Geometry g = *gp;
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
			double epsilon = 1e-5;
			if (smallestY < (pos.y + epsilon) && biggestY > (pos.y - epsilon) &&
					smallestX < (pos.x + epsilon) && biggestX > (pos.x - epsilon)) {
				Intersection hit = Intersection();
				hit.pos = pos;
				hit.o = g.type;
				hit.g = &g;
				hits.push_back(hit);
			}
		}

		// evaluate which hit is closest
		Intersection finalHit;
		double distance = 1000e6;
		for (Intersection i : hits) {
			if (rayOrigin.distance(i.pos) < distance) {
				finalHit = i;
				distance = rayOrigin.distance(i.pos);
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
