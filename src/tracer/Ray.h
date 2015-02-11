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
			double previousRefractiveIndex = 1.0;
			double originalAngle = 0.0;
			int tracings = 0;
			double signalPower = 0.0;
			int rayNumber = 0;
			enum waveBehaviour {
				wave_reflection,
				wave_refraction,
				wave_transmission,
				wave_absorption,
				wave_no_propagation,
				wave_none,
				wave_tracing_limit_exceeded
			};
			waveBehaviour behaviour;
			static constexpr double magnitude = 900;
			static constexpr int TRACING_LIMIT = 1000;
			static constexpr double powerTransmitted = 10.0; 	// [W]
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
