/*
 * Vector3d.cpp
 */

#include <cmath>
#include "Vector3d.h"

namespace raytracer {
namespace math {

	Vector3d::Vector3d() {

		x = y = z = 0.0;
	}

	Vector3d::Vector3d(double xPos, double yPos, double zPos) {

		x = xPos;
		y = yPos;
		z = zPos;
	}

	double Vector3d::magnitude() {

		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	/**
	 * Dot product between this vector and another vector v2
	 */
	double Vector3d::dot(Vector3d v2) {

		return x*v2.x + y*v2.y + z*v2.z;
	}

	/**
	 * Cross product between this vector and another vector v2
	 *
	 * vector result = s1i + s2j + s3k = u × v
	 */
	Vector3d Vector3d::cross(Vector3d v2) {

		Vector3d result = Vector3d();

		result.x = y*v2.z - z*v2.y;
		result.y = z*v2.x - x*v2.z;
		result.z = x*v2.y - y*v2.x;

		return result;
	}

	double Vector3d::distance(Vector3d v2) {

		return sqrt(pow(x - v2.x, 2) + pow(y - v2.y, 2) + pow(z - v2.z, 2));
	}

} /* namespace math */
} /* namespace raytracer */
