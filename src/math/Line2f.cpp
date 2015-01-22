/*
 * Line2f.cpp
 *
 *  Created on: 9 Jan 2015
 *      Author: rian
 */

#include "Line2f.h"
#include "Vector2f.h"

namespace raytracer {
namespace math {

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
		intersection.x = ((begin.x * end.y - begin.y * end.x) * (line.begin.x - line.end.x)
			- (begin.x - end.x) * (line.begin.x * line.end.y - line.begin.y * line.end.x))
			/((begin.x - end.x) * (line.begin.y - line.end.y) - (begin.y - end.y) * (line.begin.x - line.end.x));

		intersection.y = ((begin.x * end.y - begin.y * end.x) * (line.begin.y - line.end.y)
				- (begin.y - end.y) * (line.begin.x * line.end.y - line.begin.y * line.end.x))
				/((begin.x - end.x) * (line.begin.y - line.end.y) - (begin.y - end.y) * (line.begin.x - line.end.x));


		return intersection;
	}

} /* namespace math */
} /* namespace raytracer */

