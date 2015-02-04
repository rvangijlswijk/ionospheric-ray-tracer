/*
 * Line2f.cpp
 *
 *  Created on: 9 Jan 2015
 *      Author: rian
 */

#include <iostream>
#include <cmath>
#include "Line2f.h"
#include "Vector2f.h"

namespace raytracer {
namespace math {

	using namespace std;

	Line2f::Line2f() {

	}

	Line2f::Line2f(Vector2f bg, Vector2f e) {

		begin = bg;
		end = e;
	}

	Vector2f Line2f::getVector() {

		Vector2f v;
		v.x = end.x - begin.x;
		v.y = end.y - begin.y;

		return v;
	}

	/**
	 * Find intersection point of this line with another line
	 */
	Vector2f Line2f::intersect(Line2f line) {

		Vector2f intersection;

		// Store the values for fast access and easy
		// equations-to-code conversion
		double x1 = begin.x, x2 = end.x, x3 = line.begin.x, x4 = line.end.x;
		double y1 = begin.y, y2 = end.y, y3 = line.begin.y, y4 = line.end.y;

		// calculate determinant
		double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		// If the determinant is zero, there is no intersection
		if (d == 0) {
			return intersection;
		}

		// Get the x and y
		double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
		double x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
		double y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

		// Check if the x and y coordinates are within both lines
		double epsilon = 1e-5;
		if (x < (min(x1, x2) - epsilon) || x > (max(x1, x2) + epsilon) ||
			x < (min(x3, x4) - epsilon) || x > (max(x3, x4) + epsilon)) {
		        return intersection;
		}
		if (y < (min(y1, y2) - epsilon) || y > (max(y1, y2) + epsilon) ||
			y < (min(y3, y4) - epsilon) || y > (max(y3, y4) + epsilon)) {
		        return intersection;
		}

		// Return the point of intersection
		intersection.x = x;
		intersection.y = y;

		return intersection;
	}

	/**
	 *
	 */
	double Line2f::angularDifference(Line2f line) {

		double thisSlope = (end.y - begin.y)/(end.x - begin.x);
		double otherSlope = (line.end.y - line.begin.y)/(line.end.x - line.begin.x);
		double a = abs((thisSlope - otherSlope)/(1 + thisSlope * otherSlope));
		return atan(a);

	}

} /* namespace math */
} /* namespace raytracer */

