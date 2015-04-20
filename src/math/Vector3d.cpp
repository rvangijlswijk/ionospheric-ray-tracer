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

	/**
	 * Magnitude of the vector
	 */
	double Vector3d::magnitude() {

		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	/**
	 * Unit vector representation
	 */
	Vector3d Vector3d::norm() {

		Vector3d newV;

		newV.x = x/magnitude();
		newV.y = y/magnitude();
		newV.z = z/magnitude();

		return newV;
	}

	/**
	 * Dot product between this vector and another vector v2
	 */
	double Vector3d::dot(Vector3d v2) {

		return x*v2.x + y*v2.y + z*v2.z;
	}

	/**
	 * Multiply this vector with a constant value
	 */
	Vector3d Vector3d::multiply(double t) {

		Vector3d newV;
		newV.x = x*t;
		newV.y = y*t;
		newV.z = z*t;

		return newV;
	}

	/**
	 * Divide this vector by a constant value
	 */
	Vector3d Vector3d::divide(double t) {

		Vector3d newV;
		newV.x = x/t;
		newV.y = y/t;
		newV.z = z/t;

		return newV;
	}

	/**
	 * Add vector v2 to this vector
	 */
	Vector3d Vector3d::add(Vector3d v2) {

		Vector3d newV;
		newV.x = x + v2.x;
		newV.y = y + v2.y;
		newV.z = z + v2.z;

		return newV;
	}

	/**
	 * Substract vector v2 from this vector
	 */
	Vector3d Vector3d::substract(Vector3d v2) {

		Vector3d newV;
		newV.x = x - v2.x;
		newV.y = y - v2.y;
		newV.z = z - v2.z;

		return newV;
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

	/**
	 * Euclidian distance between two 3D-points. The vectors are in this
	 * case regarded as points in 3D space
	 */
	double Vector3d::distance(Vector3d v2) {

		return sqrt(pow(x - v2.x, 2) + pow(y - v2.y, 2) + pow(z - v2.z, 2));
	}

	/**
	 * Return the angle theta between two vectors A and B where
	 * theta = acos(A*B / (A.magnitude()*B.magnitude()))
	 */
	double Vector3d::angle(Vector3d v2) {

		return acos(this->dot(v2) / (this->magnitude() * v2.magnitude()));
	}

	Vector3d Vector3d::EQUINOX = Vector3d(1, 0, 0);
	Vector3d Vector3d::SUBSOLAR = Vector3d(0, 1, 0);
	Vector3d Vector3d::POLAR = Vector3d(0, 0, 1);

} /* namespace math */
} /* namespace raytracer */
