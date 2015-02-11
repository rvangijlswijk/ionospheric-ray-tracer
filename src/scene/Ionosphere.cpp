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
	void Ionosphere::interact(Ray *r, Vector2d &hitpos) {

		refract(r, hitpos);

		attenuate(r);

		Data d;
		d.x = r->o.x;
		d.y = r->o.y;
		d.mu_r_sqrt = pow(r->previousRefractiveIndex, 2);
		d.n_e = getElectronNumberDensity();
		d.omega_p = getPlasmaFrequency();
		d.theta_0 = r->originalAngle;
		d.frequency = r->frequency;
		d.signalPower = r->signalPower;
		Application::getInstance().addToDataset(d);
	}

	void Ionosphere::refract(Ray *r, Vector2d &hitpos) {

		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);

		double SZA = getSolarZenithAngle2d();
		double beta = atan(r->d.y/r->d.x);
		double theta_i = getIncidentAngle(r);

		int waveBehaviour = determineWaveBehaviour(r);

		if (waveBehaviour == Ray::wave_reflection) {
			r->behaviour = Ray::wave_reflection;
			double beta_r = - beta - 2*SZA;
			r->setAngle(beta_r);
//			cout << "Reflect this ray! beta_r:" << beta_r * 180 / Constants::PI << " d.x,d.y:" << r->d.x << "," << r->d.y << "\n";
		} else if (waveBehaviour == Ray::wave_refraction) {
			if (r->d.y < 0) {
				theta_i = Constants::PI - theta_i;
			}
			r->behaviour = Ray::wave_refraction;
			double theta_r = asin((r->previousRefractiveIndex/refractiveIndex * sin(theta_i)));
			if (theta_r > 0.9 * Constants::PI/2) {
//				cerr << "Bend this ray! refraction: theta_i:" << theta_i * 180 / Constants::PI << ", theta_r:" << theta_r * 180 / Constants::PI << endl;
			}
			double beta_2 = Constants::PI/2 - theta_r - SZA;
			if (r->d.y < 0) {
//				cerr << "Ray going down! r->d.y:" << r->d.y << ", beta_2:" << beta_2;
				beta_2 = -Constants::PI/2 + theta_r - SZA;
				r->setAngle(beta_2);
//				cerr << " changed to r->d.y:" << r->d.y << ",beta_2:" << beta_2 << endl;
			}
			r->setAngle(beta_2);
//			cout << "Bend this ray! refraction: theta_i:" << theta_i * 180 / Constants::PI << ", theta_r:" << theta_r * 180 / Constants::PI << " \n";
		} else if (waveBehaviour == Ray::wave_none) {
			r->behaviour = Ray::wave_none;
//			cout << "Ray goes straight!\n";
		} else {
			cerr << "No idea what to do with this ray!";
		}
		r->o = hitpos;
		r->previousRefractiveIndex = refractiveIndex;
	}

	/**
	 * A radio wave is attenuated by its propagation through the ionosphere.
	 * The attenuation calculation is based on Nielsen, 2007. Attenuation depends
	 * on the electron-neutral collision frequenc, the altitude w.r.t the peak
	 * production altitude and the radio wave frequency. The result is in db/m
	 * and thus is internally converted to a dimensionless attenuation factor,
	 * taking the layer height and the ray angle into account.
	 * A ray with a higher SZA will travel a longher path through the layer and
	 * thus face more attenuation.
	 */
	void Ionosphere::attenuate(Ray *r) {

		double collisionFrequency = 1e6 * exp(-(getAltitude() - 90000) / Constants::NEUTRAL_SCALE_HEIGHT);

		double attenuationDb = 4.61e-5 * getElectronNumberDensity() * (collisionFrequency
				/(pow(2 * Constants::PI * r->frequency, 2) + pow(collisionFrequency, 2)))
				* layerHeight / sin(r->getAngle()); // attenuation in [db/m]

		printf("colfreq: %4.2e, freq: %2.1e, att.: %4.2e, ", collisionFrequency, r->frequency, attenuationDb);

		r->signalPower /= pow(10, attenuationDb/10);
	}

	/**
	 * A radio wave is attenuated by its propagation through the ionosphere.
	 * The attenuation calculation is based on Withers, 2011. The ray frequencies
	 * are considered intermediate frequencies with neither rayFreq << plasmaFreq and
	 * plasmaFreq << rayFreq. Thus, the altitude w.r.t scale height depends what
	 * attenuation factor (A) needs to be used.
	 * Possible cases:
	 *  A = P_lo if (z0 - zl)/H < -1
	 *  A = minimum[P_lo, P_hi] if (z0 - zl)/H > -1
	 * Subsequently: P = P_t / A where P is the resulting power
	 */
	void Ionosphere::attenuateWithers(Ray *r) {

		double zL = Constants::NEUTRAL_SCALE_HEIGHT * log(Ionosphere::surfaceCollisionFrequency
				/ (2 * Constants::PI * r->frequency));

		printf("zL: %4.2e", zL);

		double pLowDb = 8.69 * (1/cos(getSolarZenithAngle2d())) * ((Constants::NEUTRAL_SCALE_HEIGHT * Ionosphere::maximumProductionRate)
				/(2 * Constants::ELECTRON_MASS * Constants::C * Constants::PERMITTIVITY_VACUUM * r->frequency * 2 *Constants::PI));

		pLowDb = 7.3e-10 * (1/cos(getSolarZenithAngle2d())) * Constants::NEUTRAL_SCALE_HEIGHT * 1e9 / r->frequency;
		double pLowW =  pow(10, pLowDb/10);
		printf("pLowDb: %4.2e, pLowW: %4.2e ", pLowDb, pLowW);

		if (Ionosphere::peakProductionAltitude < zL - Constants::NEUTRAL_SCALE_HEIGHT) {
			r->signalPower /= pLowDb;
		} else {
			double pHigh = 8.69 * (1/cos(getSolarZenithAngle2d())) * ((Constants::NEUTRAL_SCALE_HEIGHT *
					Ionosphere::peakProductionAltitude * pow(Constants::ELEMENTARY_CHARGE, 2) * Ionosphere::surfaceCollisionFrequency)
					/(2 * Constants::ELECTRON_MASS * Constants::C * pow(2 * Constants::PI * r->frequency,2)));
			printf("pHigh: %4.2e, ", pHigh);
			if (pLowDb < pHigh) r->signalPower /= pLowW;
			else r->signalPower /= pHigh;
		}
	}

	/**
	 * Calculate the plasma frequency which depends on the electron number density
	 * which depends on the altitude (y). Use a chapman profile.
	 * @unit: rad s^-1
	 */
	double Ionosphere::getPlasmaFrequency() {

		if (!_plasmaFrequency.isset()) {
			_plasmaFrequency.set(
				sqrt(getElectronNumberDensity() * pow(Constants::ELEMENTARY_CHARGE, 2)
				/ (Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM)));
		}

		return _plasmaFrequency.get();
	}

	/**
	 * Use a chapmanProfile to calculate the electron number density
	 * @unit: particles m^-3
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
	double Ionosphere::getRefractiveIndex(Ray *r, refractiveMethod m) {

		double n = 1.0;

		if (m == REFRACTION_SIMPLE) {

			n = sqrt(1 - getPlasmaFrequency() / (2 * Constants::PI * r->frequency));
		} else if (m == REFRACTION_KELSO) {

			n = sqrt(1 - getElectronNumberDensity() * pow(Constants::ELEMENTARY_CHARGE, 2) /
								(Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM * pow(2 * Constants::PI * r->frequency,2)));
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
	double Ionosphere::getIncidentAngle(Ray *r) {

		double SZA = getSolarZenithAngle2d();
		double beta = atan(r->d.y/r->d.x);
		double theta_i = Constants::PI/2 - beta - SZA;

		return theta_i;
	}

	/**
	 * A wave is either reflected or refracted depending on its incident angle. According to
	 * Snells' law, a critical angle exists for which the reflected angle is 90 deg. Incident
	 * angles above this critical angle are reflected, not refracted.
	 */
	int Ionosphere::determineWaveBehaviour(Ray *r) {

		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);
		double criticalAngle;
		double incidentAngle = getIncidentAngle(r);

		if (incidentAngle > Constants::PI/2)
			incidentAngle -= Constants::PI/2;

		if (refractiveIndex <= r->previousRefractiveIndex)
			criticalAngle = asin(refractiveIndex / r->previousRefractiveIndex);
		else
			criticalAngle = asin(r->previousRefractiveIndex / refractiveIndex);

		if (incidentAngle >= criticalAngle)
			return Ray::wave_reflection;
		else
			return Ray::wave_refraction;

		return Ray::wave_none;
	}

} /* namespace scene */
} /* namespace raytracer */
