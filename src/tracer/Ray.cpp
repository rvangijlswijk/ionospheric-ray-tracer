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
#include "../core/Config.h"

namespace raytracer {
namespace tracer {

	using namespace scene;
	using namespace core;
	using namespace exporter;
	using namespace math;

	Ray::Ray() {
		behaviour = Ray::wave_none;
		lastHit = (Geometry*)malloc(sizeof(Geometry));
	}

	/**
	 * Trace a ray recursively using the whitted-style raytracing algorithm.
	 * Raytracing stops once a ray hits the ground
	 */
	int Ray::trace() {

		if (std::isnan(o.x) || std::isnan(o.y)) {
			cout << "n: " << previousRefractiveIndex << endl;
			cerr << "NaN exception!" << endl;
			return 0;
		}

		// extrapolate a line from the ray start and its direction
		Line2d rayLine;
		Vector2d rayEnd;
		double angle = atan2(d.y, d.x);
		rayLine.begin = o;
		rayEnd.x = o.x + Ray::magnitude * cos(angle);
		rayEnd.y = o.y + Ray::magnitude * sin(angle);
		rayLine.end = rayEnd;

//		printf("Tracing ray: %6.3f %6.3f %6.3f %6.3f theta: %4.2f\n", o.x, o.y, d.x, d.y, angle * 57.296);

		// find intersection
		Intersection hit = Application::getInstance().getSceneManager().intersect(this, rayLine);
		this->lastHit = hit.g;

		// calculate time-of-flight
		if (hit.o != Geometry::none) {
			calculateTimeOfFlight(hit.pos);
		} else {
			calculateTimeOfFlight(rayEnd);
		}

//		cout << "rayline: (" << rayLine.end.x << "," << rayLine.end.y << ") ";
//		cout << "previndex: " << previousRefractiveIndex << "\n";

		// limit the simulation to avoid unnecessary calculations
		if (rayLine.begin.distance(Vector2d(0,0)) > Application::getInstance().getCelestialConfig().getInt("radius") + 250e3) {
			cerr << "Out of scene bounds!" << endl;
			return 0;
		}
		if (tracings >= Application::getInstance().getApplicationConfig().getInt("tracingLimit")) {
			cerr << "Tracing limit exceeded!" << endl;
			return 0;
		}

		Application::getInstance().incrementTracing();
		tracings++;

		// determine ray behaviour
		// intersection with an ionospheric or atmospheric layer
		if (hit.o == Geometry::ionosphere || hit.o == Geometry::atmosphere) {
//			cout << "result: ionosphere" << endl;
			hit.g->interact(this, hit.pos);
			if (behaviour == Ray::wave_no_propagation) {
				return 0;
			} else {
				return trace();
			}
		} else if (hit.o == Geometry::terrain) {
			o = rayLine.end;
			Data dataset;
			dataset.rayNumber = rayNumber;
			dataset.x = o.x;
			dataset.y = o.y;
			dataset.theta_0 = originalAngle;
			dataset.frequency = frequency;
			dataset.signalPower = signalPower;
			dataset.timeOfFlight = timeOfFlight;
			dataset.collisionType = Geometry::terrain;
			Application::getInstance().addToDataset(dataset);
			cout << "result: terrain\n";
//			printf("Intersection with terrain at: %6.2f, %6.2f\n", hit.pos.x, hit.pos.y);
//			printf("Geometry coords: %8.4f %8.4f %8.4f %8.4f\n", hit.g.getMesh().begin.x, hit.g.getMesh().begin.y, hit.g.getMesh().end.x, hit.g.getMesh().end.y);
			return 0;
		} else if (hit.o == Geometry::none) {
//			cout << "result: none\n";
			o = rayLine.end;
			Data dataset;
			dataset.rayNumber = rayNumber;
			dataset.x = o.x;
			dataset.y = o.y;
			dataset.theta_0 = originalAngle;
			dataset.frequency = frequency;
			dataset.signalPower = signalPower;
			dataset.timeOfFlight = timeOfFlight;
			dataset.collisionType = Geometry::none;
			Application::getInstance().addToDataset(dataset);
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

	double Ray::getAngle() {

		return atan2(d.y, d.x);
	}

	/**
	 * Set the direction of the ray in radians
	 */
	void Ray::setAngle(double angleRad) {

		d.x = cos(angleRad);
		d.y = sin(angleRad);
	}

	void Ray::calculateTimeOfFlight(Vector2d rayEnd) {

		double magnitude = o.distance(rayEnd);

		timeOfFlight += magnitude / Constants::C;
	}

	/**
	 * Perform an instance copy of this ray
	 */
	Ray Ray::copy() {

		Ray r2;
		r2.previousRefractiveIndex = previousRefractiveIndex;
		r2.originalAngle = originalAngle;
		r2.frequency = frequency;
		r2.tracings = tracings;

		return r2;
	}

} /* namespace tracer */
} /* namespace raytracer */
