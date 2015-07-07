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
#include "../core/Application.h"
#include "../core/Config.h"

namespace raytracer {
namespace scene {

	using namespace std;
	using namespace core;

	SceneManager::SceneManager() {}

	/**
	 *  Load celestial and application configuration values
	 */
	void SceneManager::loadStaticEnvironment() {
		BOOST_LOG_TRIVIAL(debug) << "test SceneManager";

		const Json::Value ionosphereConfig = Application::getInstance().getCelestialConfig().getObject("ionosphere");
		dh = ionosphereConfig["step"].asInt();
		minH = ionosphereConfig["start"].asInt();
		maxH = ionosphereConfig["end"].asInt();
		R = Application::getInstance().getCelestialConfig().getInt("radius");
		angularStepSize = Application::getInstance().getApplicationConfig().getDouble("angularStepSize");
	}

	/**
	 * Find which object in the scene intersects with a ray
	 */
	Intersection SceneManager::intersect(Ray &r, Line3d & rayLine) {

		static int foo = 0;

		Intersection finalHit;
		//finalHit.g = new Geometry();//(Geometry*)malloc(sizeof(Geometry));
		finalHit.o = GeometryType::none;
		bool goingUp = rayLine.origin.distance(Vector3d::CENTER) < rayLine.destination.distance(Vector3d::CENTER);

		double nextAlt;

		if (goingUp) {
			nextAlt = rayLine.origin.distance(Vector3d::CENTER) - R + dh;
		} else {
			nextAlt = rayLine.origin.distance(Vector3d::CENTER) - R - dh;
		}

		Vector3d oldNormal;
		if (r.lastHitType == GeometryType::none) {
			oldNormal = r.o.norm();
		} else {
			oldNormal = r.lastHitNormal;
		}

		BOOST_LOG_TRIVIAL(debug) << "Rayline intercept: " << rayLine.getVector() << ", old normal: " << oldNormal << ", colType: " << r.lastHitType;
		BOOST_LOG_TRIVIAL(debug) << "curAlt: " << r.altitude << ", nextAlt:" << nextAlt;

		if (nextAlt >= minH && nextAlt <= maxH) {
			foo++;
			BOOST_LOG_TRIVIAL(debug) << "Use instant approach: goingup=" << goingUp;
			double DA = R + r.altitude;
			double DB = R + nextAlt;
			// in x-y plane
			Vector3d v1 = Vector3d(r.d.x, r.d.y, r.d.z);
			Vector3d v2 = Vector3d(oldNormal.x, oldNormal.y, oldNormal.z);
			double theta, dR;
			if (goingUp) {
				theta = Constants::PI - v1.angle(v2);
				dR = sqrt(pow(DA, 2) * pow(cos(theta) ,2) - pow(DA, 2) + pow(DB, 2)) + DA * cos(theta);
			} else {
				theta = v1.angle(v2);
				dR = -sqrt(pow(DA, 2) * pow(cos(theta) ,2) - pow(DA, 2) + pow(DB, 2)) - DA * cos(theta);
			}
			Vector3d dRv = r.o + r.d * dR;
			BOOST_LOG_TRIVIAL(debug) << "dRv: " << dRv;
			BOOST_LOG_TRIVIAL(debug) << "DA: " << DA << ", DB: " << DB;

			BOOST_LOG_TRIVIAL(debug) << "normal created: " << dRv.norm();
			BOOST_LOG_TRIVIAL(debug) << "r.d: " << r.d << " -> theta_i: " << r.d.angle(dRv.norm()) * 180.0 / Constants::PI;

			// construct new ionosphere object
			Plane3d mesh = Plane3d(dRv.norm(), dRv);
			mesh.size = angularStepSize * R;
			Ionosphere* io = new Ionosphere(mesh);
			io->layerHeight = dh;

			const Json::Value ionosphereConfig = Application::getInstance().getCelestialConfig().getObject("ionosphere");
			for (int idx = 0; idx < ionosphereConfig["layers"].size(); idx++) {

				double electronPeakDensity = atof(ionosphereConfig["layers"][idx].get("electronPeakDensity", "").asCString());
				double peakProductionAltitude = ionosphereConfig["layers"][idx].get("peakProductionAltitude", "").asDouble();
				double neutralScaleHeight = ionosphereConfig["layers"][idx].get("neutralScaleHeight", 11.1e3).asDouble();

				io->superimposeElectronNumberDensity(electronPeakDensity, peakProductionAltitude, neutralScaleHeight);
			}
			BOOST_LOG_TRIVIAL(debug) << "Object created: " << io->mesh3d.centerpoint << " with alt: " << io->mesh3d.centerpoint.distance(Vector3d::CENTER) - R;

			finalHit.pos = io->mesh3d.centerpoint;
			finalHit.g = io;
			finalHit.o = GeometryType::ionosphere;
//			if (foo == 6) {
//				std::exit(0);
//			}
			io = nullptr;

		} else {
			BOOST_LOG_TRIVIAL(debug) << "Use collision detection approach";
			Vector3d pos;
			list<Intersection*> hits = list<Intersection*>();
			double epsilon = 1e-5;

			for (Geometry* gp : _sceneObjectsVector) {

				Plane3d mesh = gp->getMesh();
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

						Intersection* hit = new Intersection();
						hit->pos = pos;
						hit->o = gp->type;
						hit->g = gp;
						hits.push_back(hit);
					}
				}
			}

			if (hits.size() > 0) {
				// evaluate which hit is closest
				double distance = 1e9;

				for (Intersection* i : hits) {
					if (r.o.distance(i->pos) < distance && r.lastHitNormal != i->g->mesh3d.normal) {
						finalHit = *i;
						distance = r.o.distance(i->pos);
					}
				}
			}
		}

		BOOST_LOG_TRIVIAL(debug) << finalHit.o << "\t" << finalHit.pos << "\t";

		return finalHit;
	}

	/**
	 * Add an object to the scene
	 */
	void SceneManager::addToScene(Geometry* obj) {

		_sceneObjectsVector.push_back(obj);
	}

	/**
	 * Return a list of all objects in the scene
	 */
	vector<Geometry*> SceneManager::getScene() {

		return _sceneObjectsVector;
	}

	/**
	 * Remove all objects currently defined in the scene
	 */
	void SceneManager::removeAllFromScene() {

		_sceneObjectsVector.clear();
	}

	/**
	 * Sort all the objects in the scene by altitude for easier lookup
	 */
	void SceneManager::sortScene() {

		std::sort(_sceneObjectsVector.begin(), _sceneObjectsVector.end(), Geometry::Compare());
	}

} /* namespace scene */
} /* namespace raytracer */
