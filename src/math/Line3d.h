//============================================================================
// Name        : Line3d.h
// Author      : Rian van Gijlswijk
// Description : Represents a line in 3D space according to L = O + tD
//				 where O is origin and D destination
//============================================================================

#ifndef LINE3D_H_
#define LINE3D_H_

#include "Vector3d.h"
#include "Plane3d.h"
#include "Nullable.h"

namespace raytracer {
namespace math {

class Line3d {

	public:
		Line3d();
		Line3d(Vector3d o, Vector3d d);
		Vector3d getVector();
		Vector3d origin;
		Vector3d destination;

		/**
		 * Find intersection point of this line with a 3D plane
		 * @param Plane3d plane the plane to intersect with
		 */
		Vector3d intersect(Plane3d plane);
};

} /* namespace math */
} /* namespace raytracer */

#endif /* LINE3D_H_ */
