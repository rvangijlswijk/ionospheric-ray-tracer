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
		float errorMargin = 0.01;
		float refractiveIndex = getRefractiveIndex(r, Ionosphere::KELSO);

		cout << "ionosphere: omega_p=" << getPlasmaFrequency(r) << ", n_e=" << getElectronNumberDensity(r) << ", h=" << getAltitude() << "\n";
		cout << "mu_r: " << refractiveIndex << "\n";

		// no propagation
		if (refractiveIndex <= -errorMargin) {
			r2.behaviour = Ray::no_propagation;
			cout << "no propagation for this ray!\n";
		// reflection
		/*} else if (-errorMargin < refractiveIndex && refractiveIndex < errorMargin) {
			r2.behaviour = Ray::reflection;
			r2.d.x = r.d.x;
			r2.d.y = -r.d.y;
			cout << "reflect this ray!\n";*/
		// refraction ??
		} else {
			cout << "previndex2: " << r.previousRefractiveIndex << "\n";
			r2.behaviour = Ray::refraction;
			if (r.previousRefractiveIndex > 0) {
				float groundAngle = Constants::PI/2 - atan2(r.d.y, r.d.x);
				float newAngle = asin((r.previousRefractiveIndex/refractiveIndex * sin(groundAngle)));
				r2.setSolarZenithAngle(newAngle);
				cout << "Bend this ray! refraction: theta_i:" << groundAngle * 180 / Constants::PI << ", theta_r:" << newAngle * 180 / Constants::PI << " \n";
				r2.previousRefractiveIndex = refractiveIndex;
				// r2.d.x = cos(newAngle);
				// r2.d.y = sin(newAngle);
			} else {
				r2.d = r.d;
				r2.previousRefractiveIndex = r.previousRefractiveIndex;
				cout << "Ray goes straight!\n";
			}
		}

		Data d;
		d.x = r.o.x;
		d.y = r.o.y;
		d.mu_r_sqrt = pow(refractiveIndex, 2);
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

		return sqrt(Ionosphere::maximumProductionRate * pow(Constants::ELEMENTARY_CHARGE, 2) / (Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM));
	}

	/**
	 * Use a chapmanProfile to calculate the electron number density
	 */
	float Ionosphere::getElectronNumberDensity(Ray &r) {

		float angle = abs(atan2(r.d.y, r.d.x));
		float normalizedHeight = (getAltitude() - Ionosphere::peakProductionAltitude) / Constants::NEUTRAL_SCALE_HEIGHT;

		return Ionosphere::maximumProductionRate * exp(0.5f * (1.0f - normalizedHeight - (1.0 / cos(Application::getInstance().originalAngle)) * exp(-normalizedHeight) ));
	}

	/**
	 * Compute the plasma refractive index according to the Appleton-Hartree
	 * dispersion relation
	 */
	float Ionosphere::getRefractiveIndex(Ray &r, refractiveMethod m) {

		float n = 1.0;

		if (m == SIMPLE) {

			n = sqrt(1 - getPlasmaFrequency(r) / (2 * Constants::PI * r.frequency));
		} else if (m == KELSO) {

			n = sqrt(1 - getElectronNumberDensity(r) * pow(Constants::ELEMENTARY_CHARGE, 2) /
								(Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM * pow(2 * Constants::PI * r.frequency,2)));
		} else if (m == AHDR) {

			//n = 1 - X / (1);
		}

		return n;
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
