//============================================================================
// Name        : Ray.cpp
// Author      : Rian van Gijlswijk
// Description : Ray
//============================================================================

#include <iostream>
#include <cmath>
#include "Ray.h"
#include "../scene/SceneManager.h"
#include "Intersection.h"
#include "../core/Application.h"
#include "../scene/Ionosphere.h"
#include "../exporter/Data.h"
#include "../math/Constants.h"

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace scene;
	using namespace core;
	using namespace exporter;
	using namespace math;

	Ray::Ray() {
		// TODO Auto-generated constructor stub
		frequency = 5e6;
		behaviour = Ray::none;
	}

	/**
	 * Trace a ray recursively using the whitted-style raytracing algorithm.
	 * Raytracing stops once a ray hits the ground
	 */
	int Ray::trace() {

		if (isnan(o.x) || isnan(o.y)) {
			return 0;
		}

		// extrapolate a line from the ray start and its direction
		Line2f rayLine;
		Vector2f rayEnd;
		float angle = atan2(d.y, d.x);
		rayLine.begin = o;
		rayEnd.x = o.x + Ray::magnitude * cos(angle);
		rayEnd.y = o.y + Ray::magnitude * sin(angle);
		rayLine.end = rayEnd;

		cout << "Tracing ray " << o.x << "," << o.y << " (" << d.x << "," << d.y << ") theta=" << (angle*180.0f/3.1415) << "\n";

		// find intersection
		Intersection hit = Application::getInstance().getSceneManager().intersect(*this, rayLine);

		Ray r2;

		cout << "rayline: (" << rayLine.end.x << "," << rayLine.end.y << ") ";
		cout << "previndex: " << previousRefractiveIndex << "\n";

		if (rayLine.begin.y > 200000) {
			return 0;
		}

		// determine ray behaviour
		// intersection with an ionospheric layer
		if (hit.g.type == Geometry::ionosphere) {
			cout << "result: ionosphere\n";
			Ionosphere& gd = (Ionosphere&) hit.g;
			r2 = gd.interact(*this, hit.pos);
			if (r2.behaviour == Ray::no_propagation) {
				return 0;
			} else {
				return r2.trace();
			}
		} else if (hit.g.type == Geometry::terrain) {
			cout << "result: terrain\n";
			return 0;
		} else if (hit.g.type == Geometry::none) {
			cout << "result: none\n";
			r2.o = rayLine.end;
			r2.d = d;
			r2.previousRefractiveIndex = previousRefractiveIndex;
			r2.originalAngle = originalAngle;
			Data dataset;
			dataset.x = o.x;
			dataset.y = o.y;
			dataset.theta_0 = originalAngle;
			Application::getInstance().dataSet.push_back(dataset);
			return r2.trace();
		}

		return 1;
	}

	/**
	 * Return the direction of the ray in radians. The direction is measured
	 * with respect to zenith, i.e. a direction of 0 rad is pointing toward
	 * the sun. pi rad is pointing nadir
	 */
	float Ray::getSolarZenithAngle() {

		return Constants::PI/2.0 - atan2(d.y, d.x);
	}

	/**
	 * Set the direction of the ray in radians
	 */
	void Ray::setSolarZenithAngle(float angleRad) {

		float terrainAngle = Constants::PI/2.0 - angleRad;
		d.x = cos(terrainAngle);
		d.y = sin(terrainAngle);
	}

} /* namespace tracer */
} /* namespace raytracer */
