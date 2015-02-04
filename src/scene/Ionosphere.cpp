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

	Ionosphere::Ionosphere(Line2d mesh) : Geometry(mesh) {

		type = Geometry::ionosphere;
	}

	Ionosphere::Ionosphere(Vector2d begin, Vector2d end) : Geometry(begin, end) {

		type = Geometry::ionosphere;
	}

	/**
	 * Interaction between ray and ionospheric layer
	 */
	Ray Ionosphere::interact(Ray &r, Vector2d &hitpos) {

		Ray r2;
		r2.o = hitpos;
		double errorMargin = 1e-2;
		double refractiveIndex = getRefractiveIndex(r, Ionosphere::KELSO);

		cout << "ionosphere: omega_p=" << getPlasmaFrequency() << ", n_e=" << getElectronNumberDensity() << ", h=" << getAltitude();
		cout << ", mu_r: " << refractiveIndex << ", prev mu_r:" << r.previousRefractiveIndex << endl;

		double normalAngle = Constants::PI/2 - atan2(r.d.y, r.d.x);

		Line2d rayLine = Line2d(r.o, hitpos);
		double angleDifference = mesh2d.angularDifference(rayLine);
		double rayIncomingToIonosphericNormalAngle = Constants::PI/2 - angleDifference;

		if (refractiveIndex - errorMargin < sin(r.originalAngle)/*Constants::PI/2 - groundAngle < errorMargin*/) {
			r2.behaviour = Ray::reflection;
			double newAngle = Constants::PI/2- r.getNormalAngle();
			r2.setNormalAngle(newAngle);
			r2.previousRefractiveIndex = r.previousRefractiveIndex;
			cout << "Reflect this ray! theta_r:" << newAngle * 180 / Constants::PI << " d.x,d.y:" << r2.d.x << "," << r2.d.y << "\n";
		} else if (r.previousRefractiveIndex > 0) {
			r2.behaviour = Ray::refraction;
			double newAngle = asin((r.previousRefractiveIndex/refractiveIndex * sin(rayIncomingToIonosphericNormalAngle)));
			if (r.d.y < 0) {
				newAngle = Constants::PI - newAngle;
			}
			r2.setNormalAngle(newAngle);
			cout << "Bend this ray! refraction: theta_i:" << normalAngle * 180 / Constants::PI << ", theta_r:" << newAngle * 180 / Constants::PI << " \n";
			r2.previousRefractiveIndex = refractiveIndex;
			// r2.d.x = cos(newAngle);
			// r2.d.y = sin(newAngle);

		} else {
			r2.behaviour = Ray::refraction;
			r2.d = r.d;
			r2.previousRefractiveIndex = r.previousRefractiveIndex;
			cout << "Ray goes straight!\n";
		}
		r2.originalAngle = r.originalAngle;
		r2.frequency = r.frequency;

		Data d;
		d.x = r.o.x;
		d.y = r.o.y;
		d.mu_r_sqrt = pow(refractiveIndex, 2);
		d.n_e = getElectronNumberDensity();
		d.omega_p = getPlasmaFrequency();
		d.theta_0 = r.originalAngle;
		d.frequency = r.frequency;
		Application::getInstance().addToDataset(d);

		return r2;
	}

	/**
	 * Calculate the plasma frequency which depends on the electron number density
	 * which depends on the altitude (y). Use a chapman profile.
	 */
	double Ionosphere::getPlasmaFrequency() {

		return sqrt(Ionosphere::maximumProductionRate * pow(Constants::ELEMENTARY_CHARGE, 2) / (Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM));
	}

	/**
	 * Use a chapmanProfile to calculate the electron number density
	 */
	double Ionosphere::getElectronNumberDensity() {

		double normalizedHeight = (getAltitude() - Ionosphere::peakProductionAltitude) / Constants::NEUTRAL_SCALE_HEIGHT;

		return Ionosphere::maximumProductionRate *
				exp(0.5f * (1.0f - normalizedHeight - (1.0 / cos(getSolarZenithAngle2f())) * exp(-normalizedHeight) ));
	}

	/**
	 * Compute the plasma refractive index according to the Appleton-Hartree
	 * dispersion relation
	 */
	double Ionosphere::getRefractiveIndex(Ray &r, refractiveMethod m) {

		double n = 1.0;

		if (m == SIMPLE) {

			n = sqrt(1 - getPlasmaFrequency() / (2 * Constants::PI * r.frequency));
		} else if (m == KELSO) {

			n = sqrt(1 - getElectronNumberDensity() * pow(Constants::ELEMENTARY_CHARGE, 2) /
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
	double Ionosphere::getAltitude() {

		double xAvg = (mesh2d.begin.x + mesh2d.end.x)/2;
		double yAvg = (mesh2d.begin.y + mesh2d.end.y)/2;

		return sqrt(pow(xAvg, 2) + pow(yAvg, 2)) - 3390e3; // todo: move hardcoded radius to config value
	}

} /* namespace scene */
} /* namespace raytracer */
