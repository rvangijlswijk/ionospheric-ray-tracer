/**
 * MatlabExporter.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include <iostream>
#include <fstream>
#include "MatlabExporter.h"

namespace raytracer {
namespace exporter {

	using namespace std;

	MatlabExporter::MatlabExporter() {
		// TODO Auto-generated constructor stub
	}

	void MatlabExporter::dump(const char *filepath, list<Data> dataset) {

		ofstream data;
		data.open(filepath);
		while (!dataset.empty()) {
			data << dataset.front().x << ","
				<< dataset.front().y << ","
				<< dataset.front().omega_p << ","
				<< dataset.front().n_e << ","
				<< dataset.front().mu_r_sqrt << ","
				<< dataset.front().theta_0 << ","
				<< dataset.front().frequency << "\n";
			dataset.pop_front();
		}
		data.close();

	}

} /* namespace exporter */
} /* namespace raytracer */
