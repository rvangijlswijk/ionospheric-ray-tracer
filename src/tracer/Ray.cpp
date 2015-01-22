//============================================================================
// Name        : Ray.cpp
// Author      : Rian van Gijlswijk
// Description : Ray
//============================================================================

#include <cmath>
#include "Ray.h"
#include "../scene/SceneManager.h"
#include "Intersection.h"

namespace raytracer {
namespace tracer {

	using namespace scene;

	Ray::Ray() {
		// TODO Auto-generated constructor stub
	}

	void Ray::trace(list<Vector2f> &path) {

		// cout << "Tracing ray " << r.o.x << "," << r.o.y << " (" << r.d.x << "," << r.d.y << ")" << "\n";
		SceneManager scm;

		// extrapolate a line from the ray start and its direction
		Line2f rayLine;
		Vector2f rayEnd;
		float angle = atan2(d.y, d.x);
		rayLine.begin = o;
		rayEnd.x = o.x + 150.0 * cos(angle);
		rayEnd.y = o.y + 150.0 * sin(angle);
		rayLine.end = rayEnd;

		// cout << "rayline: (" << rayLine.end.x << "," << rayLine.end.y << ")\n";

		// find intersection
		Intersection hit = scm.intersect(*this, rayLine);
		path.push_back(hit.pos);

		// determine ray behaviour
		if (hit.o == Intersection::ionosphere) {
			Ray r2;
			r2.o = hit.pos;
			r2.d.x = d.x;
			r2.d.y = -d.y;
			r2.trace(path);
		} else if (hit.o == Intersection::terrain) {
			// stop?
		}
	}

} /* namespace tracer */
} /* namespace raytracer */
