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

	double Plane3d::getConstant() {

		return -normal.x * centerpoint.x - normal.y * centerpoint.y - normal.z * centerpoint.z;
	}

} /* namespace math */
} /* namespace raytracer */

