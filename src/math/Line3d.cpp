/*
 * Line3d.cpp
 */

#include <iostream>
#include <cmath>
#include "Line3d.h"

namespace raytracer {
namespace math {

	using namespace std;

	Line3d::Line3d() {}

	Line3d::Line3d(Vector3d o, Vector3d d) {

		origin = o;
		destination = d;
	}

	Vector3d Line3d::getVector() {

		return Vector3d(destination.x-origin.x, destination.y-origin.y, destination.z-origin.z);
	}

	/**
	 * Find intersection point of this line with a 3D plane
	 * @param Plane3d plane the plane to intersect with
	 */
	Vector3d Line3d::intersect(Plane3d plane) {

		Vector3d intersection;

		Vector3d ab = destination - origin;
		double t = -(plane.normal * origin + plane.getConstant()) / (ab * plane.normal);

//		std::cout << "t: " << t << " ";

		// an intersection occurs if t is nonnegative and real between 0 and 1 and the point of intersection
		// is within the dimensions of the plane: {dx, dy, dz} < plane.size/2
		if (t >= 0.0 && t <= 1.0) {
			intersection = origin + ab*t;

			double dx = std::abs(plane.centerpoint.x - intersection.x);
			double dy = std::abs(plane.centerpoint.y - intersection.y);
			double dz = std::abs(plane.centerpoint.z - intersection.z);

			if (dx > plane.size/2 || dy > plane.size/2 || dz > plane.size/2) {
				return Vector3d(0, 0, 0);
			}
		}

		return intersection;
	}

}
}
