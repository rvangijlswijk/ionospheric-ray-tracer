//============================================================================
// Name        : Ionosphere.h
// Author      : Rian van Gijlswijk
// Description : Perform interaction between a ray and an ionospheric layer
//				 based on the ionospheric properties
//============================================================================

#ifndef IONOSPHERE_H_
#define IONOSPHERE_H_

#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Ionosphere : public Geometry {

		public:
			Ionosphere();
			Ionosphere(Line2f mesh);
			Ionosphere(Vector2f begin, Vector2f end);
			enum refractiveMethod {
				SIMPLE,
				KELSO,		// According to Kelso, 1964
				AHDR		// Appleton-Hartree Dispersion Relation
			};
			Ray interact(Ray &r, Vector2f &hitpos);
			float getPlasmaFrequency();
			float getElectronNumberDensity();
			float getRefractiveIndex(Ray &r, refractiveMethod m);
			static constexpr float peakProductionAltitude = 125000.0;	// m
			static constexpr float maximumProductionRate = 2.5e11;		// m^-3
			float getAltitude();
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
