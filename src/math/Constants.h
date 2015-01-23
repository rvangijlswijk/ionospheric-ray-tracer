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
			static constexpr float ELECTRON_MASS = 9.1094e-31;
			static constexpr float ELEMENTARY_CHARGE = 1.602e-19;
			static constexpr float NEUTRAL_SCALE_HEIGHT = 11100.0;
			static constexpr float PERMITTIVITY_VACUUM = 8.8542e-12;
			static constexpr float PI = 3.1415;

	};

} /* namespace math */
} /* namespace raytracer */

#endif /* CONSTANTS_H_ */
