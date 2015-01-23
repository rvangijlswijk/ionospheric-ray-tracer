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

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace scene;
	using namespace core;
	using namespace exporter;

	Ray::Ray() {
		// TODO Auto-generated constructor stub
		frequency = 4.0e6;
		behaviour = Ray::none;
	}

	/**
	 * Trace a ray recursively using the whitted-style raytracing algorithm.
	 * Raytracing stops once a ray hits the ground
	 */
	int Ray::trace() {

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

		if (rayLine.begin.y > 150000) {
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
			Data dataset;
			dataset.x = o.x;
			dataset.y = o.y;
			Application::getInstance().dataSet.push_back(dataset);
			return r2.trace();
		}

		return 1;
	}

} /* namespace tracer */
} /* namespace raytracer */
