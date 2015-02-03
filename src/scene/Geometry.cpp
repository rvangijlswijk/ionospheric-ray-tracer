/**
 * Geometry.cpp
 */

#include <iostream>
#include <cmath>
#include "../math/Constants.h"
#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace math;

	Geometry::Geometry() {

	}

	Geometry::Geometry(Line2f mesh) {

		mesh2d = mesh;
	}

	Geometry::Geometry(Vector2f begin, Vector2f end) {

		mesh2d = Line2f(begin, end);
	}

	/**
	 * Compute the SZA with respect to the middle point of the 2D mesh.
	 * This angle depends on the spherical radius of the celestial body
	 * and the heights of the endpoints of the mesh as well as its length.
	 */
	float Geometry::getSolarZenithAngle2f() {

		float xAvg = (mesh2d.begin.x + mesh2d.end.x)/2;
		float yAvg = (mesh2d.begin.y + mesh2d.end.y)/2;

		return Constants::PI/2 - atan2(yAvg, xAvg);
	}

	Line2f Geometry::getMesh() {

		return mesh2d;
	}

	void Geometry::setMesh(Line2f mesh) {

		mesh2d = mesh;
	}

}
}
