/*
 * Worker.cpp
 *
 *  Created on: 2 Feb 2015
 *      Author: rian
 */

#include "Worker.h"

namespace raytracer {
namespace threading {

	using namespace math;
	using namespace tracer;
	using namespace boost::log;

	Worker::Worker() {

	}

	boost::thread* Worker::start(Ray r) {

		return new boost::thread(&Worker::process, this, r);
	}

	void Worker::process(Ray r) {

		BOOST_LOG_TRIVIAL(warning) << "Worker started for ray " << r.rayNumber;

		r.trace();

		BOOST_LOG_TRIVIAL(warning) << "Worker ended for ray " << r.rayNumber;
	}

	void Worker::schedule(boost::threadpool::pool *tp, Ray r) {

		tp->schedule(boost::bind(&Worker::process, this, r));
	}

} /* namespace threading */
} /* namespace raytracer */
