//============================================================================
// Name        : Intersection.h
// Author      : Rian van Gijlswijk
// Description : Contains the intersection information of the ray and an object
//============================================================================

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <stdlib.h>
#include "Ray.h"
#include "../math/Vector2d.h"
#include "../scene/Geometry.h"

namespace raytracer {
namespace tracer {

	using namespace math;
	using namespace scene;

	class Intersection {

		public:
			Intersection();
			~Intersection();
			Ray r;
			Geometry::object_type o = Geometry::none;
			Geometry* g = (Geometry*)malloc(sizeof(Geometry));
			Vector2d pos;

	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* INTERSECTION_H_ */
