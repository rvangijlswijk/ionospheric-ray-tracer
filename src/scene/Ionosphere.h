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
			Ionosphere(Line2d mesh);
			Ionosphere(Vector2d begin, Vector2d end);
			enum refractiveMethod {
				SIMPLE,
				KELSO,		// According to Kelso, 1964
				AHDR		// Appleton-Hartree Dispersion Relation
			};
			Ray interact(Ray &r, Vector2d &hitpos);
			double getPlasmaFrequency();
			double getElectronNumberDensity();
			double getRefractiveIndex(Ray &r, refractiveMethod m);
			static constexpr double peakProductionAltitude = 125000.0;	// m
			static constexpr double maximumProductionRate = 2.5e11;		// m^-3
			double getAltitude();
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
