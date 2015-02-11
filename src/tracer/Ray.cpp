//============================================================================
// Name        : Ray.cpp
// Author      : Rian van Gijlswijk
// Description : Ray
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Ray.h"
#include "../scene/SceneManager.h"
#include "Intersection.h"
#include "../core/Application.h"
#include "../scene/Ionosphere.h"
#include "../exporter/Data.h"
#include "../math/Constants.h"

namespace raytracer {
namespace tracer {

	using namespace scene;
	using namespace core;
	using namespace exporter;
	using namespace math;

	Ray::Ray() {
		// TODO Auto-generated constructor stub
		behaviour = Ray::wave_none;
	}

	/**
	 * Trace a ray recursively using the whitted-style raytracing algorithm.
	 * Raytracing stops once a ray hits the ground
	 */
	int Ray::trace() {

//		float x = (float) o.x;
//		if (std::isnan(x) || std::isnan((float) o.y)) {
//			cerr << "NaN exception!" << endl;
//			return 0;
//		}

		// extrapolate a line from the ray start and its direction
		Line2d rayLine;
		Vector2d rayEnd;
		double angle = atan2(d.y, d.x);
		rayLine.begin = o;
		rayEnd.x = o.x + Ray::magnitude * cos(angle);
		rayEnd.y = o.y + Ray::magnitude * sin(angle);
		rayLine.end = rayEnd;

//		printf("Tracing ray: %4.2e %4.2e %4.2f %4.2f theta: %4.2f\n", o.x, o.y, d.x, d.y, angle * 57.296);

		// find intersection
		Intersection hit = Application::getInstance().getSceneManager().intersect(*this, rayLine);

//		cout << "rayline: (" << rayLine.end.x << "," << rayLine.end.y << ") ";
//		cout << "previndex: " << previousRefractiveIndex << "\n";

		// limit the simulation to avoid unnecessary calculations
		if (rayLine.begin.distance(Vector2d(0,0)) > 3390e3 + 250e3) {
			cerr << "Out of scene bounds!" << endl;
			return 0;
		}
		if (tracings >= TRACING_LIMIT) {
			cerr << "Tracing limit exceeded!" << endl;
			return 0;
		}

		// determine ray behaviour
		// intersection with an ionospheric layer
		if (hit.g.type == Geometry::ionosphere) {
			Application::getInstance().incrementTracing();
			tracings++;
			//cout << "result: ionosphere\n";
			Ionosphere& gd = (Ionosphere&) hit.g;
			gd.interact(this, hit.pos);
			if (behaviour == Ray::wave_no_propagation) {
				return 0;
			} else {
				return trace();
			}
		} else if (hit.g.type == Geometry::terrain) {
			Application::getInstance().incrementTracing();
			tracings++;
			cout << "result: terrain\n";
//			printf("Geometry coords: %8.4f %8.4f %8.4f %8.4f\n", hit.g.getMesh().begin.x, hit.g.getMesh().begin.y, hit.g.getMesh().end.x, hit.g.getMesh().end.y);
			return 0;
		} else if (hit.g.type == Geometry::none) {
//			cout << "result: none\n";
			o = rayLine.end;
			Data dataset;
			dataset.rayNumber = rayNumber;
			dataset.x = o.x;
			dataset.y = o.y;
			dataset.theta_0 = originalAngle;
			dataset.frequency = frequency;
			dataset.signalPower = signalPower;
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
