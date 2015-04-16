/*
 * Line3d.cpp
 */

#include <cmath>
#include "Line3d.h"

namespace raytracer {
namespace math {

	Line3d::Line3d() {}

	Line3d::Line3d(Vector3d o, Vector3d d) {

		origin = o;
		destination = d;
	}

	/**
	 * Find intersection point of this line with a 3D plane
	 * @param Plane3d plane the plane to intersect with
	 */
	Vector3d Line3d::intersect(Plane3d plane) {

		Vector3d intersection;

		double t = -(origin.dot(plane.normal) + plane.getConstant() ) / (destination.dot(plane.normal));

		// an intersection occurs if t is nonnegative and real between 0 and 1
		if (t >= 0.0 && t <= 1.0) {
			intersection.x = origin.x + t*destination.x;
			intersection.y = origin.y + t*destination.y;
			intersection.z = origin.z + t*destination.z;
		}

		return intersection;
	}

}
}
