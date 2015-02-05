/*
 * Worker.cpp
 *
 *  Created on: 2 Feb 2015
 *      Author: rian
 */

#include "Worker.h"
#include "../tracer/Ray.h"
#include "../core/Application.h"
#include "../math/Constants.h"

namespace raytracer {
namespace threading {

	using namespace core;
	using namespace math;
	using namespace tracer;

	Worker::Worker() {

	}

	boost::thread* Worker::start(Ray r) {

		return new boost::thread(&Worker::process, this, r);
	}

	void Worker::process(Ray r) {

		cout << "Worker started" << endl;

		r.trace();

		cout << "Worker done" << endl;
	}

} /* namespace threading */
} /* namespace raytracer */
