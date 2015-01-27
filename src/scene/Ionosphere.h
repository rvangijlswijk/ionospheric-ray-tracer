/*
 * Ionosphere.h
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

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
				AHDR
			};
			Ray interact(Ray &r, Vector2f &hitpos);
			float getPlasmaFrequency(Ray &r);
			float getElectronNumberDensity(Ray &r);
			float getRefractiveIndex(Ray &r, refractiveMethod m);
			static constexpr float peakProductionAltitude = 125000.0;	// m
			static constexpr float maximumProductionRate = 2.0e11;		// m^-3

		private:
			float getAltitude();
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* IONOSPHERE_H_ */
