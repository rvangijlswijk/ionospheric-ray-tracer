/*
 * ShortDipoleAntenna.h
 *
 *  Created on: 28 Jul 2015
 *      Author: rian
 */

#ifndef RADIO_ShortDipoleANTENNA_H_
#define RADIO_ShortDipoleANTENNA_H_

#include "IAntenna.h"
#include "AntennaFactory.h"

namespace raytracer {
namespace radio {

	class ShortDipoleAntenna : public IAntenna {

		public:
			ShortDipoleAntenna();
			~ShortDipoleAntenna() {}
			void setConfig(const Json::Value conf);
			double getSignalPowerAt(double azimuth, double elevation);

		private:
			static AntennaRegister<ShortDipoleAntenna> reg;
	};


} /* namespace radio */
} /* namespace raytracer */

#endif /* RADIO_ShortDipoleANTENNA_H_ */
