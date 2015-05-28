/**
 * Geometry.cpp
 */

#include "Geometry.h"
#include "../core/Application.h"

namespace raytracer {
namespace scene {

	using namespace math;
	using namespace tracer;

	Geometry::Geometry() {

	}

	Geometry::Geometry(Plane3d mesh) {

		setMesh(mesh);
	}

	Geometry::Geometry(Vector3d n, Vector3d c) {

		mesh3d = Plane3d(n, c);

		calculateAltitude();
	}

	/**
	 * Compute the SZA with respect to the middle point of the 2D mesh.
	 * This angle depends on the spherical radius of the celestial body
	 * and the heights of the endpoints of the mesh as well as its length.
	 */
	double Geometry::getSolarZenithAngle2d() {

		return atan2(mesh3d.centerpoint.x, mesh3d.centerpoint.y);
	}

	Plane3d Geometry::getMesh() {

		return mesh3d;
	}

	void Geometry::setMesh(Plane3d mesh) {

		mesh3d = mesh;

		calculateAltitude();
	}

	void Geometry::interact(Ray *r, Vector3d &hitpos) {
		cout << "Geometry::interact" << endl;
	}

	/**
	 * Interpolate the altitude of this object.
	 */
	void Geometry::calculateAltitude() {

		if (altitude < 0) {

			altitude = sqrt(pow(mesh3d.centerpoint.x, 2) + pow(mesh3d.centerpoint.y, 2) + pow(mesh3d.centerpoint.z, 2))
					- core::Application::getInstance().getCelestialConfig().getInt("radius");
		}
	}
	double Geometry::getAltitude() {

		return altitude;
	}

}
}
