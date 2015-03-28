//============================================================================
// Name        : Ray.h
// Author      : Rian van Gijlswijk
// Description : Ray representation according to r = e + t*d
//============================================================================

#ifndef RAY_H_
#define RAY_H_

#include <list>
#include "../core/namespace.h"
#include "../math/Vector2d.h"
#include "../scene/Geometry.h"

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace math;
	using namespace scene;

	class Ray : public Vector2d {

		public:
			Ray();
			int trace();
			void calculateTimeOfFlight(Vector2d rayEnd);
			double getNormalAngle();
			void setNormalAngle(double angleRad);
			double getAngle();
			void setAngle(double angleRad);
			Ray copy();
			Vector2d o;	// origin
			Vector2d d;	// direction
			double frequency = 0.0;
			double previousRefractiveIndex = 1.0;
			double previousAtmosphericRefractiveIndex = 1.0;
			double originalAngle = 0.0;
			int tracings = 0;
			double signalPower = 0.0;
			double timeOfFlight = 0.0;
			int rayNumber = 0;
			double rangeDelay = 0.0;
			Geometry* lastHit;
			enum waveBehaviour {
				wave_reflection = 1,
				wave_refraction = 2,
				wave_transmission = 3,
				wave_absorption = 4,
				wave_no_propagation = 5,
				wave_none = 6,
				wave_tracing_limit_exceeded = 7
			};
			waveBehaviour behaviour;
			static constexpr double magnitude = 10e3;
			static constexpr int TRACING_LIMIT = 1000;
			static constexpr double powerTransmitted = 10.0; 	// [W]
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
