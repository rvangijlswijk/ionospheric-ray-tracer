//============================================================================
// Name        : SceneManager.cpp
// Author      : Rian van Gijlswijk
// Description : SceneManager
//============================================================================

#include <iostream>
#include <typeinfo>
#include <string>
#include <iostream>
#include <algorithm> // remove and remove_if
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
	Intersection SceneManager::intersect(Ray &r, Line3d & rayLine) {

		Vector3d pos;
		Intersection finalHit;
		finalHit.o = GeometryType::none;
		list<Intersection> hits;

		int quadrant = 2;
		Vector3d rt = rayLine.getVector();
		if (rt.x > 0) {
			if(rt.y < 0)
				quadrant = 4;
			else
				quadrant = 1;
		} else {
			if(rt.y < 0)
				quadrant = 3;
		}

		double epsilon = 1e-5;

		for (Geometry* gp : _sceneObjectsVector) {

			Plane3d mesh = gp->getMesh();

			double dx = r.o.x - mesh.centerpoint.x;
			double dy = r.o.y - mesh.centerpoint.y;

			if (dx < 0 && dy < 0 && quadrant == 1)
				continue;
			if (dx > 0 && dy < 0 && quadrant == 2)
				continue;
			if (dx > 0 && dy > 0 && quadrant == 3)
				continue;
			if (dx < 0 && dy > 0 && quadrant == 4)
				continue;

			pos = rayLine.intersect(mesh);

			if (abs(pos.x) > epsilon || abs(pos.y) > epsilon || abs(pos.z) > epsilon) {
				double smallestX = rayLine.origin.x;
				double biggestX = rayLine.destination.x;
				if (rayLine.destination.x < rayLine.origin.x) {
					smallestX = rayLine.destination.x;
					biggestX = rayLine.origin.x;
				}
				double smallestY = rayLine.origin.y;
				double biggestY = rayLine.destination.y;
				if (rayLine.destination.y < rayLine.origin.y) {
					smallestY = rayLine.destination.y;
					biggestY = rayLine.origin.y;
				}
				double smallestZ = rayLine.origin.z;
				double biggestZ = rayLine.destination.z;
				if (rayLine.destination.z < rayLine.origin.z) {
					smallestZ = rayLine.destination.z;
					biggestZ = rayLine.origin.z;
				}

				// is it within the scene and within the limits of the ray itself?
				if (smallestY < (pos.y + epsilon) && biggestY > (pos.y - epsilon) &&
						smallestX < (pos.x + epsilon) && biggestX > (pos.x - epsilon) &&
						smallestZ < (pos.z + epsilon) && biggestZ > (pos.z - epsilon)) {

					Intersection hit = Intersection();
					hit.pos = pos;
					hit.o = gp->type;
					hit.g = gp;
					hits.push_back(hit);
				}
			}
		}

		if (hits.size() > 0) {
			// evaluate which hit is closest
			double distance = 1e9;
			for (Intersection i : hits) {
				if (r.o.distance(i.pos) < distance && r.lastHit != i.g) {
					finalHit = i;
					distance = r.o.distance(i.pos);
				}
			}
		}

		return finalHit;
	}

	/**
	 * Add an object to the scene
	 */
	void SceneManager::addToScene(Geometry* obj) {

		//sceneObjectsList.push_back(obj);
		_sceneObjectsVector.push_back(obj);
	}

	/**
	 * Return a list of all objects in the scene
	 */
	list<Geometry*> SceneManager::getScene() {

		return sceneObjectsList;
	}

	/**
	 * Remove all objects currently defined in the scene
	 */
	void SceneManager::removeAllFromScene() {

		sceneObjectsList.clear();
	}

} /* namespace scene */
} /* namespace raytracer */
