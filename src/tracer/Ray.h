//============================================================================
// Name        : Ray.h
// Author      : Rian van Gijlswijk
// Description : Ray representation according to r = e + t*d
//============================================================================

#ifndef RAY_H_
#define RAY_H_

#include <list>
#include "../math/Vector2f.h"

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace math;

	class Ray : public Vector2f {

		public:
			Ray();
			void trace(list<Vector2f> &path);
			Vector2f o;	// origin
			Vector2f d;	// direction
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
