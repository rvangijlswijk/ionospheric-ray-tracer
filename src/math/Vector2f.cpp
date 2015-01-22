/*
 * Vector2f.cpp
 *
 *  Created on: 9 Jan 2015
 *      Author: rian
 */

#include <cmath>
#include "Vector2f.h"

namespace raytracer {
namespace math {

Vector2f::Vector2f() {

	x = 0.0;
	y = 0.0;

}

Vector2f::Vector2f(float xPos, float yPos) {

	x = xPos;
	y = yPos;
}

float Vector2f::magnitude() {

	return sqrt(pow(x, 2) + pow(y, 2));
}

float Vector2f::distance(Vector2f v2) {

	return sqrt(pow(x - v2.x, 2) + pow(y - v2.y, 2));
}

float Vector2f::dotProduct(Vector2f v2) {

	return x*v2.x + y*v2.y;
}

} /* namespace math */
} /* namespace raytracer */
