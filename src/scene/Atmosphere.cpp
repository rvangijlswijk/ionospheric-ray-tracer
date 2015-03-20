/*
 * Atmosphere.cpp
 *
 *  Created on: 19 Mar 2015
 *      Author: rian
 */

#include <cmath>
#include "Atmosphere.h"
#include "../math/Constants.h"
#include "../exporter/Data.h"
#include "../core/Application.h"

namespace raytracer {
namespace scene {

	using namespace core;
	using namespace exporter;

	Atmosphere::Atmosphere() : Geometry() {

		type = Geometry::atmosphere;
	}

	Atmosphere::Atmosphere(Vector2d begin, Vector2d end) : Geometry(begin, end) {

		type = Geometry::atmosphere;
	}

	void Atmosphere::setup() {}

	void Atmosphere::interact(Ray *r, Vector2d &hitpos) {

		refract(r, hitpos);

		exportData(r);
	}

	/**
	 * Refraction from ITU Recommendation P.453-6
	 */
	void Atmosphere::refract(Ray *r, Vector2d &hitpos) {

		double refractiveIndex = getRefractiveIndex();
		double theta_i = getIncidentAngle(r);
		double SZA = getSolarZenithAngle2d();
		if (r->d.y < 0) {
			theta_i = Constants::PI - theta_i;
		}
		double theta_r = asin(r->previousAtmosphericRefractiveIndex/refractiveIndex * sin(theta_i));
		double beta_2 = Constants::PI/2 - theta_r - SZA;

//		cout << "Atmo: theta_i: " << theta_i * 57.296 << ", theta_r: " << theta_r * 57.296 << endl;

		if (r->d.y < 0) {
			beta_2 = -Constants::PI/2 + theta_r - SZA;
		}
		r->setAngle(beta_2);
//		cout << "Atmo: beta_2: " << beta_2 * 57.296 << endl;

		r->o.x = hitpos.x;
		r->o.y = hitpos.y;
		r->previousAtmosphericRefractiveIndex = refractiveIndex;
	}

	void Atmosphere::attenuate(Ray *r, double magnitude) {}

	void Atmosphere::exportData(Ray *r) {

		Data d;
		d.x = r->o.x;
		d.y = r->o.y;
		d.rayNumber = r->rayNumber;
		d.theta_0 = r->originalAngle;
		d.frequency = r->frequency;
		d.signalPower = r->signalPower;
		d.collisionType = Geometry::atmosphere;
		Application::getInstance().addToDataset(d);
	}

	/**
	 * Interpolate the altitude of this ionospheric layer. Throw an exception
	 * if the difference between the y-components of the edges of the mesh is
	 * too large, for this would imply the ionospheric layer is under an angle
	 * which does not make sense.
	 */
	double Atmosphere::getAltitude() {

		double xAvg = (mesh2d.begin.x + mesh2d.end.x)/2;
		double yAvg = (mesh2d.begin.y + mesh2d.end.y)/2;

		return sqrt(pow(xAvg, 2) + pow(yAvg, 2)) - Application::getInstance().getCelestialConfig().getInt("radius");
	}

	/**
	 * The incident angle of a ray with respect to the atmospheric layer. This angle depends
	 * on the propagation angle of the ray and the angle of the layer w.r.t. the sun (SZA)
	 */
	double Atmosphere::getIncidentAngle(Ray *r) {

		double SZA = getSolarZenithAngle2d();
		double beta = atan(r->d.y/r->d.x);
		double theta_i = Constants::PI/2 - beta - SZA;

//		cout << "Atmo: SZA: " << SZA  * 57.296 << ", beta: " << beta * 57.296 << endl;

		return theta_i;
	}

	/**
	 * Refractive index as defined in Ho,2002
	 */
	double Atmosphere::getRefractiveIndex() {

		double N0 = 3.9;
		double N = N0 * exp(-getAltitude() / Constants::NEUTRAL_SCALE_HEIGHT);
		return 1 + N * 1e-6;
	}

} /* namespace scene */
} /* namespace raytracer */
