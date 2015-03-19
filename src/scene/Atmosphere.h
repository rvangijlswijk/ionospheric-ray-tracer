//============================================================================
// Name        : Atmosphere.h
// Author      : Rian van Gijlswijk
// Description : Perform interaction between a ray and an atmospheric layer
//				 based on the atmospheric properties
//============================================================================

#ifndef ATMOSPHERE_H_
#define ATMOSPHERE_H_

#include "Geometry.h"
#include "../math/Line2d.h"

namespace raytracer {
namespace scene {

	using namespace tracer;
	using namespace math;

	class Atmosphere : public Geometry {

		public:
			Atmosphere();
			Atmosphere(Line2d mesh);
			Atmosphere(Vector2d begin, Vector2d end);
			void setup();
			void interact(Ray *r, Vector2d &hitpos);
			void refract(Ray *r, Vector2d &hitpos);
			void attenuate(Ray *r, double magnitude);
			void exportData(Ray *r);
			double getAltitude();
			double getIncidentAngle(Ray *r);
			double getRefractiveIndex();
		};

} /* namespace threading */
} /* namespace raytracer */

#endif /* ATMOSPHERE_H_ */
