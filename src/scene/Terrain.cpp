/*
 * Terrain.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include "Terrain.h"

namespace raytracer {
namespace scene {

	Terrain::Terrain() {

		type = Geometry::terrain;
	}

	Terrain::Terrain(Line2f mesh) : Geometry(mesh) {

		type = Geometry::terrain;
	}

	Terrain::Terrain(Vector2f begin, Vector2f end) : Geometry(begin, end) {

		type = Geometry::terrain;
	}

	Ray Terrain::interact(Ray &r, Vector2f &hitpos) {

		Ray r2;
		return r2;
	}

} /* namespace scene */
} /* namespace raytracer */
