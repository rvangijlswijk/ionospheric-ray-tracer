//============================================================================
// Name        : Line2f.h
// Author      : Rian van Gijlswijk
// Description : Represents a line in 2D space
//============================================================================

#ifndef LINE2F_H_
#define LINE2F_H_

#include "Vector2d.h"

namespace raytracer {
namespace math {

class Line2d {

	public:
		Line2d();
		Line2d(Vector2d bg, Vector2d e);
		Vector2d begin;
		Vector2d end;
		Vector2d getVector();
		Vector2d intersect(Line2d line);
		Vector2d getCenterPoint();
		double angularDifference(Line2d line);
};

} /* namespace math */
} /* namespace raytracer */

#endif /* LINE2F_H_ */
