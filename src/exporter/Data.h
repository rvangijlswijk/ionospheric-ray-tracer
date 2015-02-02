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
			float x = 0;
			float y = 0;
			float omega_p = 0;
			float n_e = 0;
			float mu_r_sqrt = 0;
			float theta_0 = 0;
			float frequency = 0;
	};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* DATA_H_ */
