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
			 * Set the parameters of this antenna using a json configuration file
			 */
			virtual void setConfig(const Json::Value conf) = 0;

			/**
			 * get the signal power at a specific azimuth/elevation angle. This
			 * is influenced by the radiation pattern
			 */
			virtual double getSignalPowerAt(double azimuth, double elevation) = 0;

			double getNominalSignalPower() {

				return _nominalSignalPower;
			}

			void setNominalSignalPower(double p) {

				_nominalSignalPower = p;
			}

		protected:
			double _nominalSignalPower = 0.0;
			Json::Value _conf;
	};

}
}

#endif /* RADIO_IANTENNA_H_ */
