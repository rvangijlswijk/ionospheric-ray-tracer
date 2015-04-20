/*
 * Line2f.cpp
 *
 *  Created on: 9 Jan 2015
 *      Author: rian
 */

#include <iostream>
#include <cmath>
#include "Plane3d.h"
#include "Vector2d.h"

namespace raytracer {
namespace math {

	using namespace std;

	Plane3d::Plane3d() {

	}

	Plane3d::Plane3d(Vector3d n, Vector3d c) {

		normal = n;
		centerpoint = c;
	}

	/**
	 * A plane is described by a(x-x0) + b(y-y0) + c(z-z0) + d = 0.
	 * The constant d is then d = -(a*x0 + b*y0 + c*z0)
	 */
	double Plane3d::getConstant() {

		return -normal.x * centerpoint.x - normal.y * centerpoint.y - normal.z * centerpoint.z;
	}

	/**
	 * The antinormal points downwards from the plane, i.e. in opposite
	 * direction of the normal
	 */
	Vector3d Plane3d::antiNormal() {

		return (normal * -1);
	}

} /* namespace math */
} /* namespace raytracer */

