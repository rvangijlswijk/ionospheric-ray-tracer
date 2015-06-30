/*
 * CommandLine.cpp
 *
 *  Created on: 30 Jun 2015
 *      Author: rian
 */

#include "CommandLine.h"

namespace raytracer {
namespace core {

	void CommandLine::addToHeader(const char* msg) {

		std::cout << msg << "\n";
	}

	void CommandLine::updateBody(const char* msg) {

		std::cout << "\r" << msg << std::flush;
	}

} /* namespace core */
} /* namespace raytracer */
