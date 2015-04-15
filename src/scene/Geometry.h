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
			virtual void interact(Ray *r, Vector3d &hitpos);
			Plane3d getMesh();
			void setMesh(Plane3d mesh);
			double getSolarZenithAngle2d();
			GeometryType type = none;
			Plane3d mesh3d;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* GEOMETRY_H_ */
