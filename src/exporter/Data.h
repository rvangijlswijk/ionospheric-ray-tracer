/*
 * Data.h
 *
 *  Created on: 23 Jan 2015
 *      Author: rian
 */

#ifndef DATA_H_
#define DATA_H_

namespace raytracer {
namespace exporter {

	class Data {

		public:
			Data();
			double x = 0;
			double y = 0;
			double omega_p = 0;
			double n_e = 0;
			double mu_r_sqrt = 0;
			double theta_0 = 0;
			double frequency = 0;
	};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* DATA_H_ */
