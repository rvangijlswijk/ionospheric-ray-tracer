//============================================================================
// Name        : Line2f.h
// Author      : Rian van Gijlswijk
// Description : Represents a line in 2D space
//============================================================================

#ifndef LINE2F_H_
#define LINE2F_H_

#include "Vector2f.h"

namespace raytracer {
namespace math {

class Line2f {

	public:
		Line2f();
		Line2f(Vector2f bg, Vector2f e);
		Vector2f begin;
		Vector2f end;
		Vector2f getVector();
		Vector2f intersect(Line2f line);
};

} /* namespace math */
} /* namespace raytracer */

#endif /* LINE2F_H_ */
