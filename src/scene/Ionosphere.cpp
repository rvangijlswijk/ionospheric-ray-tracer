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
#include "GeometryType.h"
#include "../math/Constants.h"
#include "../exporter/Data.h"
#include "../core/Application.h"
#include "../core/Config.h"
#include "../math/NormalDistribution.h"

namespace raytracer {
namespace scene {

	using namespace std;
	using namespace exporter;
	using namespace core;

	Ionosphere::Ionosphere() : Geometry() {

		type = GeometryType::ionosphere;
	}

	Ionosphere::Ionosphere(Plane3d mesh) : Geometry(mesh) {

		type = GeometryType::ionosphere;
	}

	Ionosphere::Ionosphere(Vector3d n, Vector3d c) : Geometry(n, c) {

		type = GeometryType::ionosphere;
	}

	/**
	 * Precalculate fixed values for this ionospheric layer. Assume static ionosphere as the
	 * time periods that we're interested in are an order of magnitude lower than the processes
	 * of photochemical reactions (discussion with Cyril Simon Wedlund)
	 */
	void Ionosphere::setup() {

		_altitude = getAltitude();
	}

	/**
	 * Interaction between ray and ionospheric layer
	 */
	void Ionosphere::interact(Ray *r, Vector3d &hitpos) {

		setup();

		double magnitude = r->o.distance(hitpos);

		refract(r, hitpos);
		attenuate(r, magnitude);
		rangeDelay(r);
		phaseAdvance(r);
		timeDelay(r);

		exportData(r);
	}

	/**
	 *
	 */
	void Ionosphere::refract(Ray *r, Vector3d &hitpos) {

		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);
		double SZA = getSolarZenithAngle2d();
		double beta = atan(r->d.y/r->d.x);
		double theta_i = getIncidentAngle(r);
		int waveBehaviour = determineWaveBehaviour(r);

		if (waveBehaviour == Ray::wave_reflection) {
//			double beta_r = - beta - 2*SZA;
//			r->setAngle(beta_r);
			r->d.x = r->d.x + 2 * cos(theta_i) * mesh3d.normal.x;
			r->d.y = r->d.y + 2 * cos(theta_i) * mesh3d.normal.y;
			r->d.z = r->d.z + 2 * cos(theta_i) * mesh3d.normal.z;
			cout << "Reflect this ray! d.x,d.y:" << r->d.x << "," << r->d.y << "\n";
		} else if (waveBehaviour == Ray::wave_refraction) {
			cout << "r.d: " << "V3D (" << r->d.x << "," << r->d.y << "," << r->d.z << ")" << "; N:"
					<< "V3D (" << mesh3d.normal.x << "," << mesh3d.normal.y << "," << mesh3d.normal.z << ")" << "\n";
//			if (r->d.y < 0) {
//				theta_i = Constants::PI - theta_i;
//			}
			double ratio = r->previousRefractiveIndex/refractiveIndex;
			double coefficient = ratio * cos(theta_i) - sqrt(1 - pow(ratio, 2) * (1 - pow(cos(theta_i), 2)));

			r->d.x = ratio * r->d.x + coefficient * mesh3d.normal.x;
			r->d.y = ratio * r->d.y + coefficient * mesh3d.normal.y;
			r->d.z = ratio * r->d.z + coefficient * mesh3d.normal.z;
			cout << "r.d: " << "V3D (" << r->d.x << "," << r->d.y << "," << r->d.z << ")" << "; N:"
					<< "V3D (" << mesh3d.normal.x << "," << mesh3d.normal.y << "," << mesh3d.normal.z << ")" << "\n";
			double theta_r = asin(abs(r->d * mesh3d.normal) / (r->d.magnitude() * mesh3d.normal.magnitude()));
//			double beta_2 = Constants::PI/2 - theta_r - SZA;
//			if (r->d.y < 0) {
//				beta_2 = -Constants::PI/2 + theta_r - SZA;
//			}
			cout << "Bend this ray! refraction: " << r->previousRefractiveIndex/refractiveIndex << " theta_i:" << theta_i * 180 / Constants::PI << ", theta_r:" << theta_r * 180 / Constants::PI << " \n";
		} else if (waveBehaviour == Ray::wave_none) {
			r->behaviour = Ray::wave_none;
			cout << "Ray goes straight!\n";
		} else {
			cerr << "No idea what to do with this ray!";
		}
		r->o.x = hitpos.x;
		r->o.y = hitpos.y;
		r->o.z = hitpos.z;
		r->previousRefractiveIndex = refractiveIndex;
	}

	/**
	 * A radio wave is attenuated by its propagation through the ionosphere.
	 * The attenuation calculation is based on Nielsen, 2007. Attenuation depends
	 * on the electron-neutral collision frequency, the altitude w.r.t the peak
	 * production altitude and the radio wave frequency. The result is in db/m
	 * and thus is internally converted to attenuation in db, taking the layer
	 * height and the ray angle into account.
	 * A ray with a higher SZA will travel a longer path through the layer and
	 * thus face more attenuation.
	 */
	void Ionosphere::attenuate(Ray *r, double magnitude) {

		double correctionFactor = 1e-2;

//		double theta_r = r->getAngle() - Constants::PI/2 + getSolarZenithAngle2d();
//
//		double mu_r = sqrt(1 - pow(getPlasmaFrequency(), 2) / pow(2 * Constants::PI * r->frequency, 2));
//
//		double ki = (-pow(getPlasmaFrequency(), 2) / (2 * Constants::C * mu_r))
//				* getCollisionFrequency() / (pow(2 * Constants::PI * r->frequency, 2) + pow(getCollisionFrequency(), 2));

//		printf("wp: %4.2e, f: %4.2e, mu_r: %4.2e, colFreq: %4.2e ", getPlasmaFrequency(), r->frequency, mu_r, getCollisionFrequency());

//		double loss = - abs(20 * log10(exp(1)) * ki * magnitude * abs(1 / cos(getSolarZenithAngle2d())) * cos(abs(theta_r)));
		double loss = -1.15e-6 * (getElectronNumberDensity() * getCollisionFrequency() * magnitude) / pow(r->frequency, 2)
				* correctionFactor;

//		printf("magnitude: %4.2f, totalLoss: %4.2e, theta: %4.2f, alt: %4.2f ", magnitude, r->signalPower, theta_r, _altitude);

//		printf("Loss: %4.2e ", loss);

		r->signalPower += loss;
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

		double pLowDb = 8.69 * (1/cos(getSolarZenithAngle2d())) * ((Constants::NEUTRAL_SCALE_HEIGHT * getElectronPeakDensity())
				/(2 * Constants::ELECTRON_MASS * Constants::C * Constants::PERMITTIVITY_VACUUM * r->frequency * 2 *Constants::PI));

		pLowDb = 7.3e-10 * (1/cos(getSolarZenithAngle2d())) * Constants::NEUTRAL_SCALE_HEIGHT * 1e9 / r->frequency;
		double pLowW =  pow(10, pLowDb/10);
		printf("pLowDb: %4.2e, pLowW: %4.2e ", pLowDb, pLowW);

		if (_peakProductionAltitude < zL - Constants::NEUTRAL_SCALE_HEIGHT) {
			r->signalPower /= pLowDb;
		} else {
			double pHigh = 8.69 * (1/cos(getSolarZenithAngle2d())) * ((Constants::NEUTRAL_SCALE_HEIGHT *
					_peakProductionAltitude * pow(Constants::ELEMENTARY_CHARGE, 2) * Ionosphere::surfaceCollisionFrequency)
					/(2 * Constants::ELECTRON_MASS * Constants::C * pow(2 * Constants::PI * r->frequency,2)));
			printf("pHigh: %4.2e, ", pHigh);
			if (pLowDb < pHigh) r->signalPower /= pLowW;
			else r->signalPower /= pHigh;
		}
	}

	/**
	 * Range delay. Data from Ho, 2002.
	 * @ todo: check formulas
	 * @ unit: m
	 */
	void Ionosphere::rangeDelay(Ray *r) {

		r->rangeDelay += 0.403 * getTEC() / pow(r->frequency, 2);
	}

	/**
	 * Phase advance. Data from Ho, 2002.
	 * @ todo: check formulas
	 * @ unit: rad
	 */
	void Ionosphere::phaseAdvance(Ray *r) {

		r->phaseAdvance += (8.44e-7 / r->frequency ) * getTEC();
	}

	/**
	 * Time delay. Data from Ho, 2002.
	 * @ todo: check formulas
	 * @ unit: sec
	 */
	void Ionosphere::timeDelay(Ray *r) {

		r->timeDelay += (1.34e-7 / pow(r->frequency, 2)) * getTEC();
	}

	void Ionosphere::exportData(Ray *r) {

		Data d;
		d.x = r->o.x;
		d.y = r->o.y;
		d.rayNumber = r->rayNumber;
		d.mu_r_sqrt = pow(r->previousRefractiveIndex, 2);
		d.n_e = getElectronNumberDensity();
		d.omega_p = getPlasmaFrequency();
		d.theta_0 = r->originalAngle;
		d.frequency = r->frequency;
		d.signalPower = r->signalPower;
		d.timeOfFlight = r->timeOfFlight;
		d.collisionType = GeometryType::ionosphere;
		Application::getInstance().addToDataset(d);
	}

	/**
	 * Return a randomly distributed electron peak density. The electron peak randomly varies with a certain
	 * standard deviation. The mean and standard deviation are based on the normal peak electron density and
	 * delta ne as given by [Gurnett, 2009].
	 * @unit: particles m^-3
	 */
	double Ionosphere::getElectronPeakDensity() {

		return _electronPeakDensity;
	}

	/**
	 * Calculate the plasma frequency which depends on the electron number density
	 * which depends on the altitude (y). Use a chapman profile.
	 * @unit: rad s^-1
	 */
	double Ionosphere::getPlasmaFrequency() {

		return sqrt(getElectronNumberDensity() * pow(Constants::ELEMENTARY_CHARGE, 2)
				/ (Constants::ELECTRON_MASS * Constants::PERMITTIVITY_VACUUM));
	}

	/**
	 * Use a chapmanProfile to calculate the electron number density
	 * @unit: particles m^-3
	 */
	double Ionosphere::getElectronNumberDensity() {

		double normalizedHeight = (_altitude - _peakProductionAltitude) / Constants::NEUTRAL_SCALE_HEIGHT;

		return getElectronPeakDensity() *
				exp(0.5f * (1.0f - normalizedHeight - (1.0 / cos(getSolarZenithAngle2d())) * exp(-normalizedHeight) ));
	}

	/**
	 * Compute the plasma refractive index. Three refractive methods are supplied:
	 * - SIMPLE:
	 * - KELSO: The simplified method as described in Kelso, 1964, p.208
	 * - AHDR: Refractive index according to the Appleton-Hartree
	 * dispersion relation. Only applies to a magnetized cold plasma.
	 */
	double Ionosphere::getRefractiveIndex(Ray *r, refractiveMethod m) {

		double n = 1.0;

		if (m == REFRACTION_KELSO) {

			n = sqrt(1 - getPlasmaFrequency() / (2 * Constants::PI * r->frequency));
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

		return sqrt(pow(mesh3d.centerpoint.x, 2) + pow(mesh3d.centerpoint.y, 2) + pow(mesh3d.centerpoint.z, 2))
				- Application::getInstance().getCelestialConfig().getInt("radius");
	}

	/**
	 * The incident angle of a ray with respect to the ionospheric layer. This angle depends
	 * on the propagation angle of the ray and the angle of the layer w.r.t. the sun (SZA)
	 * The angle is then complementary to the angle between the ray direction vector and the
	 * normal of the plane.
	 */
	double Ionosphere::getIncidentAngle(Ray *r) {

//		double SZA = getSolarZenithAngle2d();
//		double beta = atan(r->d.y/r->d.x);
//		double theta_i = Constants::PI/2 - beta - SZA;

		return asin(abs(r->d * mesh3d.normal) / (r->d.magnitude() * mesh3d.normal.magnitude()));
	}

	/**
	 * Model the collision frequency according to Nielsen 2007, figure 4. This is an interpolated
	 * approximation and therefore only valid between 30 and 200km altitude.
	 */
	double Ionosphere::getCollisionFrequency() {

		return Ionosphere::surfaceCollisionFrequency * exp(-_altitude / Constants::NEUTRAL_SCALE_HEIGHT);
	}

	/**
	 * Calculate the total electron content
	 *  (TEC) which this ray experiences as it passes through
	 * the ionosphere.
	 */
	double Ionosphere::getTEC() {

		return getElectronNumberDensity() * layerHeight;
	}

	/**
	 * A wave is either reflected or refracted depending on its incident angle. According to
	 * Snells' law, a critical angle exists for which the reflected angle is 90 deg. Incident
	 * angles above this critical angle are reflected, not refracted.
	 * @ todo: REWRITE COMPLETELY
	 */
	int Ionosphere::determineWaveBehaviour(Ray *r) {

		r->behaviour = Ray::wave_none;

		double criticalAngle;
		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);
		double incidentAngle = getIncidentAngle(r);

		if (incidentAngle > Constants::PI/2)
			incidentAngle -= Constants::PI/2;


		if (refractiveIndex <= r->previousRefractiveIndex)
			criticalAngle = asin(refractiveIndex / r->previousRefractiveIndex);
		else
			criticalAngle = asin(r->previousRefractiveIndex / refractiveIndex);

		if (incidentAngle >= criticalAngle)
			r->behaviour = Ray::wave_reflection;
		else
			r->behaviour = Ray::wave_refraction;

		return r->behaviour;
	}

	void Ionosphere::setPeakProductionAltitude(double p) {

		_peakProductionAltitude = p;
	}

	void Ionosphere::setElectronPeakDensity(double e) {

		_electronPeakDensity = e;
	}

} /* namespace scene */
} /* namespace raytracer */
