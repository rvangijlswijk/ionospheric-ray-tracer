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
#include "../math/Nullable.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Ionosphere {

		public:
			Ionosphere();
			Ionosphere(Geometry g);
			enum refractiveMethod {
				REFRACTION_SIMPLE,
				REFRACTION_KELSO,		// According to Kelso, 1964
				REFRACTION_AHDR			// Appleton-Hartree Dispersion Relation
			};
			void interact(Ray *r, Vector2d &hitpos);
			void refract(Ray *r, Vector2d &hitpos);
			void attenuate(Ray *r, double magnitude);
			void attenuateWithers(Ray *r);
			void exportData(Ray *r);
			double getPlasmaFrequency();
			double getElectronPeakDensity();
			double getElectronNumberDensity();
			double getRefractiveIndex(Ray *r, refractiveMethod m);
			double getAltitude();
			double getIncidentAngle(Ray *r);
			double getCollisionFrequency();
			int determineWaveBehaviour(Ray *r);
			double layerHeight = 0;
			Geometry geom;
			static constexpr double peakProductionAltitude = 125000.0;	// m
			static constexpr double electronPeakDensity = 2.5e11;		// m^-3
			static constexpr double surfaceCollisionFrequency = 1e7;	// s^-1

		private:
			math::NDouble* _plasmaFrequency = new math::NDouble();
			math::NDouble* _electronNumberDensity = new math::NDouble();
			double _altitude = 0;
			double _electronPeakDensity = 0;
			double _peakProductionAltitude = 0;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
