/*
 * Worker.cpp
 *
 *  Created on: 2 Feb 2015
 *      Author: rian
 */

#include <cmath>
#include <boost/log/trivial.hpp>
#include "Worker.h"
#include "../core/Application.h"
#include "../core/CommandLine.h"

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

		BOOST_LOG_TRIVIAL(info) << "Worker started for ray " << r.rayNumber;

		r.trace();

		BOOST_LOG_TRIVIAL(info) << "Worker ended for ray " << r.rayNumber;

		if (Application::getInstance().getVerbosity() > boost::log::trivial::info) {
			char buffer[80];
			sprintf(buffer, "Progress: %d/%d (%4.2f\%)", r.rayNumber, Application::getInstance().numWorkers,
					100.0*r.rayNumber/((double)Application::getInstance().numWorkers));
			CommandLine::getInstance().updateBody(buffer);
		}
	}

	void Worker::schedule(boost::threadpool::pool *tp, Ray r) {

		tp->schedule(boost::bind(&Worker::process, this, r));
	}

} /* namespace threading */
} /* namespace raytracer */
