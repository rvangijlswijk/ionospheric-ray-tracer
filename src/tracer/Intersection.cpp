/*
 * Intersection.cpp
 *
 *  Created on: 9 Jan 2015
 *      Author: rian
 */

#include "Intersection.h"

namespace raytracer {
namespace tracer {

	Intersection::Intersection() {

		g = new Geometry();
	}

	Intersection::~Intersection() {

//		delete g;
	}


} /* namespace engine */
} /* namespace raytracer */

