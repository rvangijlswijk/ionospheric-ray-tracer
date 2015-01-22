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
		plasmaFrequency = 4e6f;
	}

	Ionosphere::Ionosphere(Line2f mesh) : Geometry(mesh) {

		type = Geometry::ionosphere;
		plasmaFrequency = 4e6f;
	}

	Ionosphere::Ionosphere(Vector2f begin, Vector2f end) : Geometry(begin, end) {

		type = Geometry::ionosphere;
		plasmaFrequency = 4e6f;
	}

	/**
	 * Interaction between ray and ionospheric layer
	 */
	Ray Ionosphere::interact(Ray &r, Vector2f &hitpos) {

		Ray r2;
		r2.o = hitpos;
		r2.d.x = r.d.x;
		r2.d.y = -r.d.y;
		return r2;
	}

} /* namespace scene */
} /* namespace raytracer */
