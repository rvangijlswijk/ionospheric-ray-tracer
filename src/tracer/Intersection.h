//============================================================================
// Name        : Intersection.h
// Author      : Rian van Gijlswijk
// Description : Contains the intersection information of the ray and an object
//============================================================================

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <cstring>
#include <stdlib.h>
#include "Ray.h"
#include "../math/Vector3d.h"
#include "../scene/Geometry.h"
#include "../scene/GeometryType.h"

namespace raytracer {
namespace tracer {

	using namespace math;
	using namespace scene;

	class Intersection {

		public:
			Intersection();
			~Intersection();
			Intersection(Intersection const& copy) {
				r = copy.r;
				o = copy.o;
				pos = copy.pos;
				memcpy(g, copy.g, sizeof g);
			}
			Intersection& operator=(const Intersection& rhs) {
				if (this != &rhs) {
					r = rhs.r;
					o = rhs.o;
					pos = rhs.pos;
					memcpy(g, rhs.g, sizeof g);
				}
				return *this;
			}
			Ray r;
			GeometryType o = GeometryType::none;
			Geometry* g;
			Vector3d pos;
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* INTERSECTION_H_ */
