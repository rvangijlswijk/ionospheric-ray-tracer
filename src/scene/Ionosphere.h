//============================================================================
// Name        : Ionosphere.h
// Author      : Rian van Gijlswijk
// Description : Perform interaction between a ray and an ionospheric layer
//				 based on the ionospheric properties
//============================================================================

#ifndef IONOSPHERE_H_
#define IONOSPHERE_H_

#include "Geometry.h"
#include "../math/NDouble.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Ionosphere : public Geometry {

		public:
			Ionosphere();
			Ionosphere(Line2d mesh);
			Ionosphere(Vector2d begin, Vector2d end);
			enum refractiveMethod {
				REFRACTION_SIMPLE,
				REFRACTION_KELSO,		// According to Kelso, 1964
				REFRACTION_AHDR			// Appleton-Hartree Dispersion Relation
			};
			void interact(Ray *r, Vector2d &hitpos);
			void refract(Ray *r, Vector2d &hitpos);
			void attenuate(Ray *r, double magnitude);
			void attenuateWithers(Ray *r);
			double getPlasmaFrequency();
			double getElectronNumberDensity();
			double getRefractiveIndex(Ray *r, refractiveMethod m);
			double getAltitude();
			double getIncidentAngle(Ray *r);
			double getCollisionFrequency();
			int determineWaveBehaviour(Ray *r);
			double layerHeight = 0;
			static constexpr double peakProductionAltitude = 125000.0;	// m
			static constexpr double maximumProductionRate = 2.5e11;		// m^-3
			static constexpr double surfaceCollisionFrequency = 1e7;	// s^-1

		private:
			math::NDouble _plasmaFrequency;
			math::NDouble _electronNumberDensity;
			math::NDouble _altitude;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
