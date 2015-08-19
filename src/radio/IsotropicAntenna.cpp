/*
 * IsotropicAntenna.cpp
 *
 *  Created on: 27 Jul 2015
 *      Author: rian
 */

#include "IsotropicAntenna.h"

namespace raytracer {
namespace radio {

	IsotropicAntenna::IsotropicAntenna() {

	}

	void IsotropicAntenna::setConfig(const Json::Value conf) {
		// isotropic, do not implement
	}

	double IsotropicAntenna::getSignalPowerAt(double azimuth, double elevation) {

		return _nominalSignalPower;
	}

	AntennaRegister<IsotropicAntenna> IsotropicAntenna::reg("IsotropicAntenna");


} /* namespace radio */
} /* namespace raytracer */
