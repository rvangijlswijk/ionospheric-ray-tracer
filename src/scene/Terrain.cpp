/*
 * Terrain.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include "Terrain.h"

namespace raytracer {
namespace scene {

	Terrain::Terrain() {}

	Terrain::Terrain(Vector2d begin, Vector2d end) : Geometry(begin, end) {
		type = Geometry::terrain;
	}

	void Terrain::interact(Ray *r, Vector2d &hitpos) {

	}

} /* namespace scene */
} /* namespace raytracer */
