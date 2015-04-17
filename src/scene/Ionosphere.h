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
			Ionosphere(Plane3d mesh);
			Ionosphere(Vector3d n, Vector3d c);
			enum refractiveMethod {
				REFRACTION_SIMPLE,
				REFRACTION_KELSO,		// According to Kelso, 1964
				REFRACTION_AHDR			// Appleton-Hartree Dispersion Relation
			};
			void setup();
			void interact(Ray *r, Vector3d &hitpos);
			void refract(Ray *r, Vector3d &hitpos);
			void attenuate(Ray *r, double magnitude);
			void attenuateWithers(Ray *r);
			void rangeDelay(Ray *r);
			void phaseAdvance(Ray *r);
			void timeDelay(Ray *r);
			void exportData(Ray *r);
			double getPlasmaFrequency();
			double getElectronPeakDensity();
			double getElectronNumberDensity();
			double getRefractiveIndex(Ray *r, refractiveMethod m);
			double getAltitude();
			double getIncidentAngle(Ray *r);
			double getCollisionFrequency();
			double getTEC();
			void setPeakProductionAltitude(double p);
			void setElectronPeakDensity(double e);
			int determineWaveBehaviour(Ray *r);
			double layerHeight = 0;
			double electronDensityVariability = 0;
			static constexpr double surfaceCollisionFrequency = 2.8e10;	// s^-1

		private:
			double _altitude = 0;
			double _peakProductionAltitude = 0;	// m
			double _electronPeakDensity = 0;		// m^-3
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
