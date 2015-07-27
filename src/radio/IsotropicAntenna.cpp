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

	double IsotropicAntenna::getSignalPowerAt(double azimuth, double elevation) {

		return 0.0;
	}

	AntennaRegister<IsotropicAntenna> IsotropicAntenna::reg("IsotropicAntenna");


} /* namespace radio */
} /* namespace raytracer */
