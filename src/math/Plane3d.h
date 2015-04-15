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

		Vector3d normal, centerpoint;
		double size;

		double getConstant();
};

} /* namespace math */
} /* namespace raytracer */

#endif /* MATH_PLANE3D_H_ */
