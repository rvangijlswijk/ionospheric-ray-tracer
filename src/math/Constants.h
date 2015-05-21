/*
 * constants.h
 *
 *  Created on: 23 Jan 2015
 *      Author: rian
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace raytracer {
namespace math {

	class Constants {

		public:
			static constexpr double ELECTRON_MASS = 9.1094e-31;
			static constexpr double ELEMENTARY_CHARGE = 1.602e-19;
			static constexpr double NEUTRAL_SCALE_HEIGHT = 9445.0;
			static constexpr double PERMITTIVITY_VACUUM = 8.8542e-12;
			static constexpr double PI = 3.1415;
			static constexpr double C = 2.9979e8;

	};

} /* namespace math */
} /* namespace raytracer */

#endif /* CONSTANTS_H_ */
