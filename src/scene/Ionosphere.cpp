/*
 * Ionosphere.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include "Ionosphere.h"
#include "../math/Constants.h"
#include "../exporter/Data.h"
#include "../core/Application.h"

namespace raytracer {
namespace scene {

	using namespace std;
	using namespace exporter;
	using namespace core;

	Ionosphere::Ionosphere() {

		type = Geometry::ionosphere;
	}

	Ionosphere::Ionosphere(Line2f mesh) : Geometry(mesh) {

		type = Geometry::ionosphere;
	}

	Ionosphere::Ionosphere(Vector2f begin, Vector2f end) : Geometry(begin, end) {

		type = Geometry::ionosphere;
	}

	/**
	 * Interaction between ray and ionospheric layer
	 */
	Ray Ionosphere::interact(Ray &r, Vector2f &hitpos) {

		Ray r2;
		r2.o = hitpos;
		float rayAngularFrequency = 2 * Constants::PI * r.frequency;
		float complexRefractiveIndex = (1 - pow(getPlasmaFrequency(r), 2) / pow(rayAngularFrequency, 2));
		float errorMargin = 0.05;

		cout << "ionosphere: omega_p=" << getPlasmaFrequency(r) << ", n_e=" << getElectronNumberDensity(r) << ", h=" << getAltitude() << "\n";
		cout << "mu_r_sqrt: " << complexRefractiveIndex << "\n";

		// no propagation
		if (complexRefractiveIndex <= -errorMargin) {
			r2.behaviour = Ray::no_propagation;
			cout << "no propagation for this ray!\n";
		// reflection
		} else if (-errorMargin < complexRefractiveIndex && complexRefractiveIndex < errorMargin) {
			r2.behaviour = Ray::reflection;
			r2.d.x = r.d.x;
			r2.d.y = -r.d.y;
			cout << "reflect this ray!\n";
		// refraction ??
		} else {
			r2.behaviour = Ray::refraction;
			r2.d = r.d;
			cout << "refract this ray!\n";
		}

		Data d;
		d.x = r.o.x;
		d.y = r.o.y;
		d.mu_r_sqrt = complexRefractiveIndex;
		d.n_e = getElectronNumberDensity(r);
		d.omega_p = getPlasmaFrequency(r);
		Application::getInstance().dataSet.push_back(d);

		return r2;
	}

	/**
	 * Calculate the plasma frequency which depends on the electron number density
	 * which depends on the altitude (y). Use a chapman profile.
	 */
	float Ionosphere::getPlasmaFrequency(Ray &r) {

		return sqrt(getElectronNumberDensity(r) * pow(Constants::ELEMENTARY_CHARGE, 2) / (Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM));
	}

	/**
	 * Use a chapmanProfile to calculate the electron number density
	 */
	float Ionosphere::getElectronNumberDensity(Ray &r) {

		float SZA = Constants::PI/2.0f - abs(atan2(r.d.y, r.d.x));
		float normalizedHeight = (getAltitude() - Ionosphere::peakProductionAltitude) / Constants::NEUTRAL_SCALE_HEIGHT;

		cout << "SZA=" << (SZA * 180 / Constants::PI) << "\n";

		return Ionosphere::maximumProductionRate * exp(0.5f * (1.0f - normalizedHeight - exp(-normalizedHeight* 1.0f/cos(SZA)) ));
	}

	/**
	 * Interpolate the altitude of this ionospheric layer. Throw an exception
	 * if the difference between the y-components of the edges of the mesh is
	 * too large, for this would imply the ionospheric layer is under an angle
	 * which does not make sense.
	 */
	float Ionosphere::getAltitude() {

		return (mesh2d.begin.y + mesh2d.end.y) / 2.0f;
	}

} /* namespace scene */
} /* namespace raytracer */
