//============================================================================
// Name        : Ray.cpp
// Author      : Rian van Gijlswijk
// Description : Ray
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Ray.h"
#include "Intersection.h"
#include "../scene/SceneManager.h"
#include "../core/Application.h"
#include "../scene/Ionosphere.h"
#include "../exporter/Data.h"
#include "../math/Constants.h"
#include "../math/Line3d.h"
#include "../core/Config.h"

namespace raytracer {
namespace tracer {

	using namespace scene;
	using namespace core;
	using namespace exporter;
	using namespace math;

	Ray::Ray() {
		behaviour = Ray::wave_none;
	}

	Ray::~Ray() {}

	/**
	 * Trace a ray recursively using the whitted-style raytracing algorithm.
	 * Raytracing stops once a ray hits the ground
	 */
	int Ray::trace() {

		// isnan check
		if (o.x != o.x || o.y != o.y) {
//			BOOST_LOG_TRIVIAL(error) << "NaN exception!";
			return 0;
		}

		// extrapolate a line from the ray start and its direction
		Line3d rayLine;
		Vector3d rayEnd;
		double angle = atan2(d.y, d.x);
		rayLine.origin = o;
		rayEnd.x = o.x + Ray::magnitude * cos(angle);
		rayEnd.y = o.y + Ray::magnitude * sin(angle);
		rayEnd.z = o.z + Ray::magnitude * d.z;
		rayLine.destination = rayEnd;

//		printf("Tracing ray: %6.3f %6.3f %6.3f %6.3f theta: %4.2f\n", o.x, o.y, d.x, d.y, angle * 57.296);

		BOOST_LOG_TRIVIAL(debug) << "rayline: (" << rayLine.destination.x << "," << rayLine.destination.y << "," << rayLine.destination.z << ") \n";
//		cout << "previndex: " << previousRefractiveIndex << "\n";

		// limit the simulation to avoid unnecessary calculations
		if (rayLine.origin.distance(Vector3d(0,0,0)) > Application::getInstance().getCelestialConfig().getInt("radius") + 250e3) {
			BOOST_LOG_TRIVIAL(info) << "Ray " << rayNumber << " result: Out of scene bounds!";

			return 0;
		}
		if (tracings >= Application::getInstance().getApplicationConfig().getInt("tracingLimit")) {
			BOOST_LOG_TRIVIAL(info) << "Ray " << rayNumber << " result: Tracing limit exceeded!";
			return 0;
		}

		// find intersection
		updateAltitude();
		Intersection hit = Application::getInstance().getSceneManager().intersect(*this, rayLine);
		lastHitType = hit.g->type;
		lastHitNormal = hit.g->mesh3d.normal;
//		printf("Hit: %6.3f, %6.3f \n", hit.pos.x, hit.pos.y);

		// calculate time-of-flight
		if (hit.o != GeometryType::none) {
			calculateTimeOfFlight(hit.pos);
		} else {
			calculateTimeOfFlight(rayEnd);
		}

		Application::getInstance().incrementTracing();
		tracings++;

		// determine ray behaviour
		// intersection with an ionospheric or atmospheric layer
		if (hit.o == GeometryType::ionosphere || hit.o == GeometryType::atmosphere) {
			hit.g->interact(this, hit.pos);
			delete hit.g;
			if (behaviour == Ray::wave_no_propagation) {
				BOOST_LOG_TRIVIAL(info) << "Ray " << rayNumber << " result: no propagation";
				return 0;
			} else {
				return trace();
			}
		} else if (hit.o == GeometryType::terrain) {
			o = rayLine.destination;
			exportData(GeometryType::terrain);
			BOOST_LOG_TRIVIAL(info) << "Ray " << rayNumber << " result: terrain";
//			printf("Intersection with terrain at: %6.2f, %6.2f\n", hit.pos.x, hit.pos.y);
//			printf("Geometry coords: %8.4f %8.4f %8.4f %8.4f\n", hit.g.getMesh().begin.x, hit.g.getMesh().begin.y, hit.g.getMesh().end.x, hit.g.getMesh().end.y);
			return 0;
		} else if (hit.o == GeometryType::none) {
			o = rayLine.destination;
			delete hit.g;
			exportData(GeometryType::none);
			return trace();
		}

		return 1;
	}

	/**
	 * Return the direction of the ray in radians. The direction is measured
	 * with respect to the normal of the ray
	 */
	double Ray::getNormalAngle() {

		return Constants::PI/2.0 - atan2(d.y, d.x);
	}

	/**
	 * Set the direction of the normal in radians
	 */
	void Ray::setNormalAngle(double angleRad) {

		double terrainAngle = Constants::PI/2.0 - angleRad;
		d.x = cos(terrainAngle);
		d.y = sin(terrainAngle);
	}

	/**
	 * Return a scalar value of the angle between direction vector and axis
	 * @param double angle
	 */
	double Ray::getAngle() {

		return d.angle(Vector3d::EQUINOX);
	}

	/**
	 * Set the direction of the ray in radians
	 */
	void Ray::setAngle(double angleRad) {

		d.x = cos(angleRad);
		d.y = sin(angleRad);
		d.z = 0;
	}

	void Ray::calculateTimeOfFlight(Vector3d rayEnd) {

		double magnitude = o.distance(rayEnd);

		timeOfFlight += magnitude / Constants::C;
	}

	void Ray::updateAltitude() {

		altitude = o.distance(Vector3d(0,0,0)) - Application::getInstance().getCelestialConfig().getInt("radius");
	}

	void Ray::exportData(GeometryType collisionType) {

		Data d;
		d.x = o.x;
		d.y = o.y;
		d.z = o.z;
		d.rayNumber = rayNumber;
		d.mu_r_sqrt = pow(previousRefractiveIndex, 2);
		d.theta_0 = originalAngle;
		d.frequency = frequency;
		d.signalPower = signalPower;
		d.timeOfFlight = timeOfFlight;
		d.collisionType = collisionType;
		d.beaconId = originBeaconId;
		d.azimuth_0 = originalAzimuth;
		Application::getInstance().addToDataset(d);
	}

} /* namespace tracer */
} /* namespace raytracer */
