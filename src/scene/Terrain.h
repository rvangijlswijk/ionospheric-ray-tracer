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
			Terrain(Line3d mesh);
			Terrain(Vector3d n, Vector3d c);
			void interact(Ray *r, Vector3d &hitpos);
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* TERRAIN_H_ */
