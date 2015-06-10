/*
 * Vector2f.cpp
 *
 *  Created on: 9 Jan 2015
 *      Author: rian
 */

#include <cmath>
#include "Vector2d.h"

namespace raytracer {
namespace math {

	Vector2d::Vector2d() {

		x = 0.0;
		y = 0.0;

	}

	Vector2d::Vector2d(double xPos, double yPos) {

		x = xPos;
		y = yPos;
	}

	double Vector2d::magnitude() {

		return sqrt(pow(x, 2) + pow(y, 2));
	}

	double Vector2d::distance(Vector2d v2) {

		return sqrt(pow(x - v2.x, 2) + pow(y - v2.y, 2));
	}

	double Vector2d::dotProduct(Vector2d v2) {

		return x*v2.x + y*v2.y;
	}

	/**
	 * Return the angle theta between two vectors A and B where
	 * theta = acos(A*B / (A.magnitude()*B.magnitude()))
	 */
	double Vector2d::angle(Vector2d v2) {

		return acos(this->dotProduct(v2) / (this->magnitude() * v2.magnitude()));
	}

} /* namespace math */
} /* namespace raytracer */
