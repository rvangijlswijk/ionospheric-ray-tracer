//============================================================================
// Name        : Geometry.h
// Author      : Rian van Gijlswijk
// Description : Base class defining basic behaviour of scene objects
//============================================================================

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../tracer/Ray.h"

namespace raytracer {
namespace scene {

using namespace tracer;

class Geometry {

	public:
		Geometry();
		void refract(Ray &r);
		void attenuate(Ray &r);
};

} /* namespace scene */
} /* namespace raytracer */

#endif /* GEOMETRY_H_ */
