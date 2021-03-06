/*
 * Data.h
 *
 *  Created on: 23 Jan 2015
 *      Author: rian
 */

#ifndef DATA_H_
#define DATA_H_

#include "../scene/GeometryType.h"

namespace raytracer {
namespace exporter {

	class Data {

		public:
			Data();
			int rayNumber = 0;
			double x = 0;
			double y = 0;
			double z = 0;
			double omega_p = 0;
			double n_e = 0;
			double mu_r_sqrt = 0;
			double theta_0 = 0;
			double azimuth_0 = 0;
			double frequency = 0;
			double signalPower = 0;
			double timeOfFlight = 0;
			int beaconId = 0;
			double aoa = 0;
			scene::GeometryType collisionType = scene::GeometryType::none;

			static constexpr int MAX_DATASET_SIZE = 100;
	};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* DATA_H_ */
