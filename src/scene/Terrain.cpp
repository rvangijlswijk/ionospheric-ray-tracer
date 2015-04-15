/*
 * Terrain.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include "Terrain.h"

namespace raytracer {
namespace scene {

	Terrain::Terrain() : Geometry() {

		type = GeometryType::terrain;
	}

	Terrain::Terrain(Vector3d n, Vector3d c) : Geometry(n, c) {

		type = GeometryType::terrain;
	}

	void Terrain::interact(Ray *r, Vector3d &hitpos) {

	}

} /* namespace scene */
} /* namespace raytracer */
