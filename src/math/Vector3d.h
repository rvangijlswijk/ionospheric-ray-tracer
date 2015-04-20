//============================================================================
// Name        : Vector3d.h
// Author      : Rian van Gijlswijk
// Description : Represents a vector in 3D space
//============================================================================

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <iostream>
#include <iomanip>
#include "Vector.h"

namespace raytracer {
namespace math {

class Vector3d {

	public:
		Vector3d();
		Vector3d(double xPos, double yPos, double zPos);

		static Vector3d EQUINOX,
						SUBSOLAR,
						POLAR;
		double x, y, z;

		/**
		 * Magnitude of the vector
		 */
		double magnitude();

		/**
		 * Unit vector representation
		 */
		Vector3d norm();

		/**
		 * Euclidian distance between two 3D-points. The vectors are in this
		 * case regarded as points in 3D space
		 */
		double distance(Vector3d v2);

		/**
		 * Cross product between this vector and another vector v2
		 *
		 * vector result = s1i + s2j + s3k = u × v
		 */
		Vector3d cross(Vector3d v2);

		/**
		 * Dot product between this vector and another vector v2
		 */
		double dot(Vector3d v2);
		double operator*(Vector3d v2) {
			return dot(v2);
		}

		/**
		 * Multiply this vector with a constant value
		 */
		Vector3d multiply(double t);
		Vector3d operator*(double t) {
			return multiply(t);
		}

		/**
		 * Divide this vector by a constant value
		 */
		Vector3d divide(double t);
		Vector3d operator/(double t) {
			return divide(t);
		}

		/**
		 * Add vector v2 to this vector
		 */
		Vector3d add(Vector3d v2);
		Vector3d operator+(Vector3d v2) {
			return add(v2);
		}

		/**
		 * Substract vector v2 from this vector
		 */
		Vector3d substract(Vector3d v2);
		Vector3d operator-(Vector3d v2) {
			return substract(v2);
		}

		/**
		 * Return the angle theta between two vectors A and B where
		 * theta = acos(A*B / (A.magnitude()*B.magnitude()))
		 */
		double angle(Vector3d v2);

		friend std::ostream& operator<<(std::ostream &strm, const raytracer::math::Vector3d &v) {

			return strm << std::fixed << std::setprecision(4) << "V3D (" << v.x << "," << v.y << "," << v.z << ")";
		}

};

} /* namespace math */
} /* namespace raytracer */

#endif /* VECTOR3D_H_ */
