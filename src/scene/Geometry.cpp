/**
 * Geometry.cpp
 */

#include <iostream>
#include "Geometry.h"

namespace raytracer {
namespace scene {

	Geometry::Geometry() {

	}

	Geometry::Geometry(Line2f mesh) {

		mesh2d = mesh;
	}

	Geometry::Geometry(Vector2f begin, Vector2f end) {

		mesh2d = Line2f(begin, end);
	}

	Line2f Geometry::getMesh() {

		return mesh2d;
	}

}
}
