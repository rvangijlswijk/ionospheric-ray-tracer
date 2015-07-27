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
			IAntenna();
			IAntenna(Json::Value config);
			virtual ~IAntenna();

			/**
			 * get the signal power at a specific azimuth/elevation angle. This
			 * is influenced by the radiation pattern
			 */
			virtual double getSignalPowerAt(double azimuth, double elevation) = 0;

			/**
			 * Register this antenna class in the antenna mapper. This has to be done
			 * so that the antenna mapper can load this class if required
			 */
			virtual void registerInMap() = 0;

		protected:

			/**
			 * The default, or isotropic, signal power, assuming no gains/losses from
			 * antenna radiation pattern
			 */
			double _isotropicSignalPower;
	};

	template<typename T> IAntenna * createInstance() { return new T; }

}
}

#endif /* RADIO_IANTENNA_H_ */
