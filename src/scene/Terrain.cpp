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

	Terrain::Terrain(Line2d mesh) : Geometry(mesh) {

		type = Geometry::terrain;
	}

	Terrain::Terrain(Vector2d begin, Vector2d end) : Geometry(begin, end) {

		type = Geometry::terrain;
	}

	Ray Terrain::interact(Ray &r, Vector2d &hitpos) {

		Ray r2;
		return r2;
	}

} /* namespace scene */
} /* namespace raytracer */
