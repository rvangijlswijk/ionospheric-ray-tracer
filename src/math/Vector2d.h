//============================================================================
// Name        : Vector2f.h
// Author      : Rian van Gijlswijk
// Description : Represents a vector in 2D space
//============================================================================

#ifndef VECTOR2F_H_
#define VECTOR2F_H_

namespace raytracer {
namespace math {

class Vector2d {

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

#endif /* VECTOR2F_H_ */
