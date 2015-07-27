/*
 * IsotropicAntenna.h
 *
 *  Created on: 27 Jul 2015
 *      Author: rian
 */

#ifndef RADIO_ISOTROPICANTENNA_H_
#define RADIO_ISOTROPICANTENNA_H_

#include "IAntenna.h"
#include "AntennaFactory.h"

namespace raytracer {
namespace radio {

	class IsotropicAntenna : public IAntenna {

		public:
			IsotropicAntenna();
			double getSignalPowerAt(double azimuth, double elevation);

		private:
			double _nominalSignalPower = 0.0;
			static AntennaRegister<IsotropicAntenna> reg;
	};


} /* namespace radio */
} /* namespace raytracer */

#endif /* RADIO_ISOTROPICANTENNA_H_ */
