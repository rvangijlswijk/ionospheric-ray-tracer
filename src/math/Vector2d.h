//============================================================================
// Name        : Vector2d.h
// Author      : Rian van Gijlswijk
// Description : Represents a vector in 2D space
//============================================================================

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include "Vector.h"

namespace raytracer {
namespace math {

class Vector2d : Vector {

	public:
		Vector2d();
		Vector2d(double xPos, double yPos);

		double x;
		double y;

		double magnitude();
		double dotProduct(Vector2d v2);
		double distance(Vector2d v2);
};

} /* namespace math */
} /* namespace raytracer */

#endif /* VECTOR2D_H_ */
