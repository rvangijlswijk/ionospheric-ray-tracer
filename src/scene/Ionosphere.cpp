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

		int waveBehaviour = determineWaveBehaviour(r);

		if (waveBehaviour == Ray::wave_reflection) {
			reflect(r);
		} else if (waveBehaviour == Ray::wave_refraction) {
			refract(r);
		}
		r->o = hitpos;

		attenuate(r);
		rangeDelay(r);
		phaseAdvance(r);
		timeDelay(r);

		exportData(r);
	}

	/**
	 *
	 */
	void Ionosphere::refract(Ray *r) {

		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);
		double theta_i = getIncidentAngle(r);

		double ratio = r->previousRefractiveIndex/refractiveIndex;
		double coefficient = ratio * cos(theta_i) - sqrt(1 - pow(ratio, 2) * (1 - pow(cos(theta_i), 2)));
		Vector3d newR = Vector3d();

		if (r->d.y > 0)
			newR = r->d * ratio - mesh3d.normal * coefficient;
		else
			newR = r->d * ratio + mesh3d.normal * coefficient;

		BOOST_LOG_TRIVIAL(debug) << std::fixed << "REFRACT Alt: " << std::setprecision(0) << getAltitude() << "\tr.d_i: " << r->d << "\tr.d_r: " << newR << "\tN: " << mesh3d.normal << "\tn1/n2: " << ratio << "\ttheta_i: " << theta_i;

		r->d = newR.norm();
		r->previousRefractiveIndex = refractiveIndex;
	}

	/**
	 *
	 */
	void Ionosphere::reflect(Ray *r) {

		double refractiveIndex = getRefractiveIndex(r, Ionosphere::REFRACTION_KELSO);
		double theta_i = getIncidentAngle(r);

		Vector3d newR = Vector3d();

		if (r->d.y > 0)
			newR = r->d - mesh3d.normal * 2 * cos(theta_i);
		else
			newR = r->d + mesh3d.normal * 2 * cos(theta_i);

		BOOST_LOG_TRIVIAL(debug) << std::fixed << "REFLECT Alt: " << std::setprecision(0) << getAltitude() << "\tr.d_i: " << r->d << "\tr.d_r: " << newR << "\tN: " << mesh3d.normal << "\ttheta_i: " << theta_i;

		r->d = newR.norm();
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
	void Ionosphere::attenuate(Ray *r) {

		double theta_r = getMesh().normal.angle(r->d);
		double magnitude = layerHeight * cos(theta_r);
		double collisionFrequency = getCollisionFrequency();

		double loss = -4.6e-5
				* (getElectronNumberDensity() * collisionFrequency / (pow(2 * Constants::PI * r->frequency, 2) + pow(collisionFrequency, 2)))
				* magnitude;

		r->signalPower += loss;
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
		d.z = r->o.z;
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

		return _electronNumberDensity;
	}

	/**
	 *
	 */
	void Ionosphere::superimposeElectronNumberDensity(double peakDensity, double peakAltitude, double neutralScaleHeight) {

		double SZA = mesh3d.normal.angle(Vector3d::SUBSOLAR);
		double correctedPeakAltitude = peakAltitude + 1e4 * log10(1/cos(SZA));
		double normalizedHeight = (getAltitude() - correctedPeakAltitude) / neutralScaleHeight;

		double electronNumberDensity = peakDensity *
				exp(0.5f * (1.0f - normalizedHeight - (1.0 / cos(SZA)) * exp(-normalizedHeight) ));

		_electronNumberDensity += electronNumberDensity;
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

			n = sqrt(1 - pow(getPlasmaFrequency(), 2) / pow(2 * Constants::PI * r->frequency, 2));
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

		if (_altitude < 1) {

			_altitude = sqrt(pow(mesh3d.centerpoint.x, 2) + pow(mesh3d.centerpoint.y, 2) + pow(mesh3d.centerpoint.z, 2))
					- Application::getInstance().getCelestialConfig().getInt("radius");
		}

		return _altitude;
	}

	/**
	 * The incident angle of a ray with respect to the ionospheric layer. This angle depends
	 * on the propagation angle of the ray and the angle of the layer w.r.t. the sun (SZA)
	 * The angle is then complementary to the angle between the ray direction vector and the
	 * normal of the plane.
	 */
	double Ionosphere::getIncidentAngle(Ray *r) {

		return acos(abs(r->d * mesh3d.normal) / (r->d.magnitude() * mesh3d.normal.magnitude()));
	}

	/**
	 * Model the collision frequency
	 * @unit Hz
	 */
	double Ionosphere::getCollisionFrequency() {

		double nCO2 = Application::getInstance().getCelestialConfig().getDouble("surfaceNCO2")
				* exp(-_altitude / Constants::NEUTRAL_SCALE_HEIGHT);
		return 1.0436e-07 * nCO2;
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
		double angularFrequency = 2 * Constants::PI * r->frequency;
		double epsilon = 1e-5;

		if (incidentAngle > Constants::PI/2)
			incidentAngle -= Constants::PI/2;


		if (refractiveIndex <= r->previousRefractiveIndex)
			criticalAngle = asin(refractiveIndex / r->previousRefractiveIndex);
		else
			criticalAngle = asin(r->previousRefractiveIndex / refractiveIndex);

		if (r->previousRefractiveIndex > refractiveIndex &&
				(incidentAngle >= criticalAngle || abs(angularFrequency - getPlasmaFrequency()) < epsilon))
			r->behaviour = Ray::wave_reflection;
		else
			r->behaviour = Ray::wave_refraction;

		return r->behaviour;
	}

} /* namespace scene */
} /* namespace raytracer */
