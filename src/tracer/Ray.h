//============================================================================
// Name        : Ray.h
// Author      : Rian van Gijlswijk
// Description : Ray representation according to r = e + t*d
//============================================================================

#ifndef RAY_H_
#define RAY_H_

#include <cstring>
#include <list>
#include "../core/namespace.h"
#include "../math/Vector3d.h"
#include "../scene/GeometryType.h"
#include "../scene/Geometry.h"

namespace raytracer {
namespace tracer {

	using namespace std;
	using namespace math;
	using namespace scene;

	class Ray {

		public:
			Ray();
			~Ray();
			int trace();
			void calculateTimeOfFlight(Vector3d rayEnd);
			double getNormalAngle();
			void setNormalAngle(double angleRad);

			/**
			 * Return a scalar value of the angle between direction vector and axis
			 * @param double angle
			 */
			double getAngle();
			void setAngle(double angleRad);
			void setAngle(Vector3d angle);
			void exportData(GeometryType collisionType);
			void updateAltitude();
//			Ray& operator=(const Ray& rhs) {
//				if (this != &rhs) {
//					memcpy(lastHit, rhs.lastHit, sizeof(lastHit));
//				}
//				return *this;
//			}

			/**
			 * Origin
			 */
			Vector3d o;

			/**
			 * Direction vector
			 */
			Vector3d d;
			double frequency = 0.0;
			double previousRefractiveIndex = 1.0;
			double previousAtmosphericRefractiveIndex = 1.0;
			double originalAngle = 0.0;
			int tracings = 0;
			double signalPower = 0.0;
			double timeOfFlight = 0.0;
			int rayNumber = 0;
			double rangeDelay = 0.0;
			double timeDelay = 0.0;
			double phaseAdvance = 0.0;
			double altitude = 0.0;
			GeometryType lastHitType;
			Vector3d lastHitNormal;
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
			static constexpr double magnitude = 50;
			static constexpr double powerTransmitted = 10.0; 	// [W]
	};

} /* namespace tracer */
} /* namespace raytracer */

#endif /* RAY_H_ */
