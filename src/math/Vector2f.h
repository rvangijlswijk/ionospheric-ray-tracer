//============================================================================
// Name        : Vector2f.h
// Author      : Rian van Gijlswijk
// Description : Represents a vector in 2D space
//============================================================================

#ifndef VECTOR2F_H_
#define VECTOR2F_H_

namespace raytracer {
namespace math {

class Vector2f {

	public:
		Vector2f();
		Vector2f(float xPos, float yPos);

		float x;
		float y;

		float magnitude();
		float dotProduct(Vector2f v2);
		float distance(Vector2f v2);
};

} /* namespace math */
} /* namespace raytracer */

#endif /* VECTOR2F_H_ */
