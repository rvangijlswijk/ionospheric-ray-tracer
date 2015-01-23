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
			int trace();
			Vector2f o;	// origin
			Vector2f d;	// direction
			float frequency;
			enum waveBehaviour {
				reflection,
				refraction,
				transmission,
				absorption,
				no_propagation,
				none
			};
			waveBehaviour behaviour;
			static constexpr float magnitude = 1000.0f;
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
