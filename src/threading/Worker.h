//============================================================================
// Name        : Worker.h
// Author      : Rian van Gijlswijk
// Description : Worker handles the raytracing of one ray. This allows multi-
//				 threading of raytracing
//============================================================================

#ifndef WORKER_H_
#define WORKER_H_

#include "../tracer/Ray.h"
#include "../../contrib/threadpool/threadpool.hpp"
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

namespace raytracer {
namespace threading {

	using namespace tracer;

	class Worker {

		public:
			Worker();
			boost::thread* start(Ray r);
			void process(Ray r);
			void schedule(boost::threadpool::pool *tp, Ray r);

		private:
			boost::thread thread;
	};

} /* namespace threading */
} /* namespace raytracer */

#endif /* WORKER_H_ */
