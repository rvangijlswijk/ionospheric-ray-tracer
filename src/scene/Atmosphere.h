//============================================================================
// Name        : Atmosphere.h
// Author      : Rian van Gijlswijk
// Description : Perform interaction between a ray and an atmospheric layer
//				 based on the atmospheric properties
//============================================================================

#ifndef ATMOSPHERE_H_
#define ATMOSPHERE_H_

#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace tracer;
	using namespace math;

	class Atmosphere : public Geometry {

		public:
			Atmosphere();
			Atmosphere(Line3d mesh);
			Atmosphere(Vector3d n, Vector3d c);
			void setup();
			void interact(Ray *r, Vector3d &hitpos);
			void refract(Ray *r, Vector3d &hitpos);
			void attenuate(Ray *r, double magnitude);
			void exportData(Ray *r);
			double getAltitude();
			double getIncidentAngle(Ray *r);
			double getRefractiveIndex();
			double layerHeight = 0;
		};

} /* namespace threading */
} /* namespace raytracer */

#endif /* ATMOSPHERE_H_ */
