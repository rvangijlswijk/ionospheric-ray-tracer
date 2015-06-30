//============================================================================
// Name        : Geometry.h
// Author      : Rian van Gijlswijk
// Description : Base class defining basic behaviour of scene objects
//============================================================================

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <iostream>
#include <cmath>
#include "../core/namespace.h"
#include "../math/Constants.h"
#include "../math/Line3d.h"
#include "../math/Plane3d.h"
#include "../math/Vector3d.h"
#include "../tracer/Ray.h"
#include "GeometryType.h"

namespace raytracer {
namespace scene {

	using namespace math;
	using namespace tracer;

	class Geometry {

		public:
			Geometry();
			Geometry(Plane3d mesh);
			Geometry(Vector3d n, Vector3d c);
			Plane3d getMesh();
			void setMesh(Plane3d mesh);

			/**
			 * Compute the SZA with respect to the middle point of the 2D mesh.
			 * This angle depends on the spherical radius of the celestial body
			 * and the heights of the endpoints of the mesh as well as its length.
			 */
			double getSolarZenithAngle2d();

			/**
			 * Interpolate the altitude of this object.
			 */
			void calculateAltitude();
			double getAltitude();
			virtual void interact(Ray *r, Vector3d &hitpos) {}
			GeometryType type = none;
			Plane3d mesh3d;
			double altitude = -1;
			bool operator<(const Geometry &rhs) const {
				return altitude < rhs.altitude;
			}

			struct Compare {
				bool operator()(const Geometry* a, const Geometry* b) {
					return a->altitude < b->altitude;
				}
			};
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* GEOMETRY_H_ */
