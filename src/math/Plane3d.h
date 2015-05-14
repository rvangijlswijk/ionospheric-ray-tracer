//============================================================================
// Name        : Plane3d.h
// Author      : Rian van Gijlswijk
// Description : Represents a plane in 3D space according to
//				 a(x-x0) + b(y-y0) + c(z-z0) = 0
//============================================================================

#ifndef MATH_PLANE3D_H_
#define MATH_PLANE3D_H_

#include "Vector3d.h"

namespace raytracer {
namespace math {

class Plane3d {

	public:
		Plane3d();
		Plane3d(Vector3d normal, Vector3d centerpoint);

		/**
		 * normal vector and centerpoint of Plane.
		 */
		Vector3d normal, centerpoint;

		/**
		 * width and height of the plane. Planes are assumed to be always square.
		 * The size dictates the boundaries of this plane. Edges of the plane are
		 * 1/2 size away from centerpoint
		 * Size is in meters
		 */
		double size = 100;

		/**
		 * A plane is described by a(x-x0) + b(y-y0) + c(z-z0) + d = 0.
		 * The constant d is then d = -(a*x0 + b*y0 + c*z0)
		 */
		double getConstant();

		/**
		 * The antinormal points downwards from the plane, i.e. in opposite
		 * direction of the normal
		 */
		Vector3d antiNormal();
};

} /* namespace math */
} /* namespace raytracer */

#endif /* MATH_PLANE3D_H_ */
