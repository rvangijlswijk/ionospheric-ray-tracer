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
		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);

		double SZA = getSolarZenithAngle2d();
		double beta = atan(r.d.y/r.d.x);
		double theta_i = getIncidentAngle(r);

		int waveBehaviour = determineWaveBehaviour(r);

		if (waveBehaviour == Ray::wave_reflection) {
			r2.behaviour = Ray::wave_reflection;
			double beta_r = - beta - 2*SZA;
			r2.setAngle(beta_r);
//			cout << "Reflect this ray! beta_r:" << beta_r * 180 / Constants::PI << " d.x,d.y:" << r2.d.x << "," << r2.d.y << "\n";
		} else if (waveBehaviour == Ray::wave_refraction) {
			if (r.d.y < 0) {
				theta_i = Constants::PI - theta_i;
			}
			r2.behaviour = Ray::wave_refraction;
			double theta_r = asin((r.previousRefractiveIndex/refractiveIndex * sin(theta_i)));
			if (theta_r > 0.9 * Constants::PI/2) {
//				cerr << "Bend this ray! refraction: theta_i:" << theta_i * 180 / Constants::PI << ", theta_r:" << theta_r * 180 / Constants::PI << endl;
			}
			double beta_2 = Constants::PI/2 - theta_r - SZA;
			if (r.d.y < 0) {
//				cerr << "Ray going down! r.d.y:" << r.d.y << ", beta_2:" << beta_2;
				beta_2 = -Constants::PI/2 + theta_r - SZA;
				r2.setAngle(beta_2);
//				cerr << " changed to r.d.y:" << r.d.y << ",beta_2:" << beta_2 << endl;
			}
			r2.setAngle(beta_2);
//			cout << "Bend this ray! refraction: theta_i:" << theta_i * 180 / Constants::PI << ", theta_r:" << theta_r * 180 / Constants::PI << " \n";
		} else if (waveBehaviour == Ray::wave_none) {
			r2.behaviour = Ray::wave_none;
			r2.d = r.d;
//			cout << "Ray goes straight!\n";
		} else {
			cerr << "No idea what to do with this ray!";
		}
		r2.o = hitpos;
		r2.previousRefractiveIndex = refractiveIndex;
		r2.originalAngle = r.originalAngle;
		r2.frequency = r.frequency;
		r2.tracings = r.tracings;

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

		if (!_plasmaFrequency.isset()) {
			_plasmaFrequency.set(
				sqrt(Ionosphere::maximumProductionRate * pow(Constants::ELEMENTARY_CHARGE, 2) / (Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM)));
		}

		return _plasmaFrequency.get();
	}

	/**
	 * Use a chapmanProfile to calculate the electron number density
	 */
	double Ionosphere::getElectronNumberDensity() {

		double normalizedHeight = (getAltitude() - Ionosphere::peakProductionAltitude) / Constants::NEUTRAL_SCALE_HEIGHT;

		return Ionosphere::maximumProductionRate *
				exp(0.5f * (1.0f - normalizedHeight - (1.0 / cos(getSolarZenithAngle2d())) * exp(-normalizedHeight) ));
	}

	/**
	 * Compute the plasma refractive index. Three refractive methods are supplied:
	 * - SIMPLE:
	 * - KELSO: The simplified method as described in Kelso, 1964, p.208
	 * - AHDR: Refractive index according to the Appleton-Hartree
	 * dispersion relation
	 */
	double Ionosphere::getRefractiveIndex(Ray &r, refractiveMethod m) {

		double n = 1.0;

		if (m == REFRACTION_SIMPLE) {

			n = sqrt(1 - getPlasmaFrequency() / (2 * Constants::PI * r.frequency));
		} else if (m == REFRACTION_KELSO) {

			n = sqrt(1 - getElectronNumberDensity() * pow(Constants::ELEMENTARY_CHARGE, 2) /
								(Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM * pow(2 * Constants::PI * r.frequency,2)));
		} else if (m == REFRACTION_AHDR) {

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

		return sqrt(pow(xAvg, 2) + pow(yAvg, 2)) - 3390e3;
	}

	/**
	 * The incident angle of a ray with respect to the ionospheric layer. This angle depends
	 * on the propagation angle of the ray and the angle of the layer w.r.t. the sun (SZA)
	 */
	double Ionosphere::getIncidentAngle(Ray &r) {

		double SZA = getSolarZenithAngle2d();
		double beta = atan(r.d.y/r.d.x);
		double theta_i = Constants::PI/2 - beta - SZA;

		return theta_i;
	}

	/**
	 * A wave is either reflected or refracted depending on its incident angle. According to
	 * Snells' law, a critical angle exists for which the reflected angle is 90 deg. Incident
	 * angles above this critical angle are reflected, not refracted.
	 */
	int Ionosphere::determineWaveBehaviour(Ray &r) {

		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);
		double criticalAngle;
		double incidentAngle = getIncidentAngle(r);

		if (incidentAngle > Constants::PI/2) {
			incidentAngle -= Constants::PI/2;
		}

		if (refractiveIndex <= r.previousRefractiveIndex) {
			criticalAngle = asin(refractiveIndex / r.previousRefractiveIndex);
		} else {
			criticalAngle = asin(r.previousRefractiveIndex / refractiveIndex);
		}

		if (incidentAngle >= criticalAngle) {
			return Ray::wave_reflection;
		} else {
			return Ray::wave_refraction;
		}
		return Ray::wave_none;
	}

} /* namespace scene */
} /* namespace raytracer */
