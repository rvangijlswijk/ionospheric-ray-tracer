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
			int trace(list<Vector2f> &path);
			Vector2f o;	// origin
			Vector2f d;	// direction
			float frequency;
			enum wave_behavior {
				reflection,
				refraction,
				transmission,
				absorption,
				none
			};
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
