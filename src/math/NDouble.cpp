/*
 * NDouble.cpp
 *
 *  Created on: 6 Feb 2015
 *      Author: rian
 */

#include <iostream>
#include "NDouble.h"

namespace raytracer {
namespace math {

	NDouble::NDouble() {}

	double NDouble::get() {

		return val;
	}

	void NDouble::set(double v) {

		val = v;
		initialized = 1;
	}

	bool NDouble::isset() {

		return initialized == 1 ? 1 : 0;
	}

	void NDouble::unset() {

		initialized = 0;
	}

} /* namespace threading */
} /* namespace raytracer */
