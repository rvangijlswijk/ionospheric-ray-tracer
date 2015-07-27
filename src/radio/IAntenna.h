/*
 * IAntenna.h
 *
 *  Created on: 27 Jul 2015
 *      Author: rian
 */

#ifndef RADIO_IANTENNA_H_
#define RADIO_IANTENNA_H_

#include "../../contrib/jsoncpp/value.h"

namespace raytracer {
namespace radio {

	class IAntenna {

		public:
			IAntenna() {}
			virtual ~IAntenna() {}

			/**
			 * get the signal power at a specific azimuth/elevation angle. This
			 * is influenced by the radiation pattern
			 */
			virtual double getSignalPowerAt(double azimuth, double elevation) {};
	};

}
}

#endif /* RADIO_IANTENNA_H_ */
