//============================================================================
// Name        : Vector3d.h
// Author      : Rian van Gijlswijk
// Description : Represents a vector in 3D space
//============================================================================

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include "Vector.h"

namespace raytracer {
namespace math {

class Vector3d {

	public:
		Vector3d();
		Vector3d(double xPos, double yPos, double zPos);

		double x, y, z;

		double magnitude();
		double dot(Vector3d v2);
		Vector3d cross(Vector3d v2);
		double distance(Vector3d v2);
};

} /* namespace math */
} /* namespace raytracer */

#endif /* VECTOR3D_H_ */
