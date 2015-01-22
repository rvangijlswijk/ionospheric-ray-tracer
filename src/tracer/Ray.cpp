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

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace scene;
	using namespace core;

	Ray::Ray() {
		// TODO Auto-generated constructor stub
	}

	/**
	 * Trace a ray recursively using the whitted-style raytracing algorithm.
	 * Raytracing stops once a ray hits the ground
	 */
	int Ray::trace(list<Vector2f> &path) {

		//cout << "Tracing ray " << o.x << "," << o.y << " (" << d.x << "," << d.y << ")" << "\n";

		// extrapolate a line from the ray start and its direction
		Line2f rayLine;
		Vector2f rayEnd;
		float angle = atan2(d.y, d.x);
		rayLine.begin = o;
		rayEnd.x = o.x + 150.0 * cos(angle);
		rayEnd.y = o.y + 150.0 * sin(angle);
		rayLine.end = rayEnd;

		//cout << "rayline: (" << rayLine.end.x << "," << rayLine.end.y << ")\n";

		// find intersection
		Intersection hit = Application::getInstance().getSceneManager().intersect(*this, rayLine);
		path.push_back(hit.pos);

		// determine ray behaviour
		if (hit.o == Geometry::ionosphere) {
			Ray r2;
			r2.o = hit.pos;
			r2.d.x = d.x;
			r2.d.y = -d.y;
			return r2.trace(path);
		} else if (hit.o == Geometry::terrain) {
			return 0;
		}

		return 1;
	}

} /* namespace tracer */
} /* namespace raytracer */
