/*
 * Terrain.h
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Terrain : public Geometry {

		public:
			Terrain();
			Terrain(Line2f mesh);
			Terrain(Vector2f begin, Vector2f end);
			Ray interact(Ray &r, Vector2f &hitpos);
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* TERRAIN_H_ */
