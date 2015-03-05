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

	Geometry::Geometry(Line2d mesh) {

		mesh2d = mesh;
	}

	Geometry::Geometry(Vector2d begin, Vector2d end) {

		mesh2d = Line2d(begin, end);
	}

	/**
	 * Compute the SZA with respect to the middle point of the 2D mesh.
	 * This angle depends on the spherical radius of the celestial body
	 * and the heights of the endpoints of the mesh as well as its length.
	 */
	double Geometry::getSolarZenithAngle2d() {

		return atan2(mesh2d.getCenterPoint().x, mesh2d.getCenterPoint().y);
	}

	Line2d Geometry::getMesh() {

		return mesh2d;
	}

	void Geometry::setMesh(Line2d mesh) {

		mesh2d = mesh;
	}

	void Geometry::interact(Ray *r, Vector2d &hitpos) {
		cout << "Geometry::interact" << endl;
	}

}
}
