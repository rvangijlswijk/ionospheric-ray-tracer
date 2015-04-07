/**
 * MatlabExporter.cpp
 *
 *  Created on: 22 Jan 2015
 *      Author: rian
 */

#include <iostream>
#include <iomanip>
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
			data << std::fixed << std::setprecision(1) << dataset.front().rayNumber << ","
				<< std::setprecision(2) << dataset.front().x << ","
				<< std::setprecision(2) << dataset.front().y << ","
				<< std::setprecision(2) << dataset.front().omega_p << ","
				<< std::setprecision(4) << dataset.front().n_e << ","
				<< std::setprecision(6) << dataset.front().mu_r_sqrt << ","
				<< std::setprecision(4) << dataset.front().theta_0 << ","
				<< std::setprecision(1) << dataset.front().frequency << ","
				<< std::setprecision(6) << dataset.front().signalPower << ","
				<< std::setprecision(10) << dataset.front().timeOfFlight << ","
				<< std::setprecision(1) << dataset.front().collisionType << "\n";
			dataset.pop_front();
		}
		data.close();

	}

} /* namespace exporter */
} /* namespace raytracer */
