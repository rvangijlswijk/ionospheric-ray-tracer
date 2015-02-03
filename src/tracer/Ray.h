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
			float getNormalAngle();
			void setNormalAngle(float angleRad);
			float getAngle();
			void setAngle(float angleRad);
			Vector2f o;	// origin
			Vector2f d;	// direction
			float frequency = 0.0;
			float previousRefractiveIndex = 0.0;
			float originalAngle = 0.0;
			enum waveBehaviour {
				reflection,
				refraction,
				transmission,
				absorption,
				no_propagation,
				none
			};
			waveBehaviour behaviour;
			static constexpr float magnitude = 2000.0f;
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
