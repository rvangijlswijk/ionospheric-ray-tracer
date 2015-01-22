//============================================================================
// Name        : Intersection.h
// Author      : Rian van Gijlswijk
// Description : Contains the intersection information of the ray and an object
//============================================================================

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "Ray.h"
#include "../math/Vector2f.h"

namespace raytracer {
namespace tracer {

using namespace math;

class Intersection {

	public:
		Intersection();

		enum object_type {
			ionosphere,
			terrain,
			none
		};

		Ray r;
		object_type o;
		Vector2f pos;

};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* INTERSECTION_H_ */
