/*
 * ShortShortDipoleAntenna.cpp
 *
 *  Created on: 28 Jul 2015
 *      Author: rian
 */

#include <cmath>
#include "ShortDipoleAntenna.h"
#include "../math/Constants.h"

namespace raytracer {
namespace radio {

	using namespace math;

	ShortDipoleAntenna::ShortDipoleAntenna() {}

	void ShortDipoleAntenna::setConfig(const Json::Value conf) {

	}

	double ShortDipoleAntenna::getSignalPowerAt(double azimuth, double elevation) {

		// rotate pattern 90 degrees
		elevation += Constants::PI / 2.0;
		if (elevation > 2 * Constants::PI)
			elevation -= 2 * Constants::PI;

		// terrain absorbs all
		if (elevation < 0)
			return 0;

		return 1.76 * pow(sin(elevation), 2);
	}

	AntennaRegister<ShortDipoleAntenna> ShortDipoleAntenna::reg("ShortDipoleAntenna");

} /* namespace radio */
} /* namespace raytracer */
