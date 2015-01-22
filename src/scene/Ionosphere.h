/*
 * Ionosphere.h
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#ifndef IONOSPHERE_H_
#define IONOSPHERE_H_

#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Ionosphere : public Geometry {

		public:
			Ionosphere();
			Ionosphere(Line2f mesh);
			Ionosphere(Vector2f begin, Vector2f end);
			Ray interact(Ray &r);
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
