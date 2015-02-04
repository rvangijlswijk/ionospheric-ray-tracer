//============================================================================
// Name        : Ray.h
// Author      : Rian van Gijlswijk
// Description : Ray representation according to r = e + t*d
//============================================================================

#ifndef RAY_H_
#define RAY_H_

#include <list>
#include "../math/Vector2d.h"

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace math;

	class Ray : public Vector2d {

		public:
			Ray();
			int trace();
			double getNormalAngle();
			void setNormalAngle(double angleRad);
			double getAngle();
			void setAngle(double angleRad);
			Ray copy();
			Vector2d o;	// origin
			Vector2d d;	// direction
			double frequency = 0.0;
			double previousRefractiveIndex = 0.0;
			double originalAngle = 0.0;
			enum waveBehaviour {
				reflection,
				refraction,
				transmission,
				absorption,
				no_propagation,
				none
			};
			waveBehaviour behaviour;
			static constexpr double magnitude = 2000.0f;
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
