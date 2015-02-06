/*
 * NDouble.cpp
 *
 *  Created on: 6 Feb 2015
 *      Author: rian
 */

#include "NDouble.h"

namespace raytracer {
namespace math {

	NDouble::NDouble() {

		hasBeenSet = false;
	}

	double NDouble::get() {

		return val;
	}

	void NDouble::set(double v) {

		val = v;
		hasBeenSet = true;
	}

	bool NDouble::isset() {

		return hasBeenSet;
	}

	void NDouble::unset() {

		hasBeenSet = false;
	}

} /* namespace threading */
} /* namespace raytracer */
