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
			float x;
			float y;
			float omega_p;
			float n_e;
			float mu_r_sqrt;
	};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* DATA_H_ */
