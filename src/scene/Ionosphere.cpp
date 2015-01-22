/*
 * Ionosphere.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include <iostream>
#include "Ionosphere.h"

namespace raytracer {
namespace scene {

	Ionosphere::Ionosphere() {

		type = Geometry::ionosphere;
	}

	Ionosphere::Ionosphere(Line2f mesh) : Geometry(mesh) {

		type = Geometry::ionosphere;
	}

	Ionosphere::Ionosphere(Vector2f begin, Vector2f end) : Geometry(begin, end) {

		type = Geometry::ionosphere;
	}

	Ray Ionosphere::interact(Ray &r) {

		Ray r2;
		return r2;
	}

} /* namespace scene */
} /* namespace raytracer */
