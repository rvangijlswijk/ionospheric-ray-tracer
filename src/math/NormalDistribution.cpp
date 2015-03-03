/*
 * NormalDistribution.cpp
 *
 *  Created on: 2 Mar 2015
 *      Author: rian
 */

#include "NormalDistribution.h"

namespace raytracer {
namespace math {

	using namespace std;

	/**
	 * Return a normally distributed random parameter which is based on a given
	 * mean and stddev
	 */
	double NormalDistribution::get(double mean, double stddev) {

		std::normal_distribution<double> dist(mean, stddev);

		return dist(gen);
	}

} /* namespace math */
} /* namespace raytracer */
