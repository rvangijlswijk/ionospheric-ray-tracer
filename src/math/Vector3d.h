//============================================================================
// Name        : Vector3d.h
// Author      : Rian van Gijlswijk
// Description : Represents a vector in 3D space
//============================================================================

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <iostream>
#include "Vector.h"

namespace raytracer {
namespace math {

class Vector3d {

	public:
		Vector3d();
		Vector3d(double xPos, double yPos, double zPos);

		double x, y, z;

		/**
		 * Magnitude of the vector
		 */
		double magnitude();

		/**
		 * Dot product between this vector and another vector v2
		 */
		double dot(Vector3d v2);
		double operator*(Vector3d v2) {
			return dot(v2);
		}

		/**
		 * Cross product between this vector and another vector v2
		 *
		 * vector result = s1i + s2j + s3k = u × v
		 */
		Vector3d cross(Vector3d v2);

		/**
		 * Euclidian distance between two 3D-points. The vectors are in this
		 * case regarded as points in 3D space
		 */
		double distance(Vector3d v2);

//		friend std::ostream& operator<<(std::ostream &strm, const raytracer::math::Vector3d &v) {
//
//			return strm << "V3D (" << v.x << "," << v.y << "," << v.z << ")";
//		}

};

} /* namespace math */
} /* namespace raytracer */

#endif /* VECTOR3D_H_ */
