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
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

namespace raytracer {
namespace exporter {

	using namespace std;

	MatlabExporter::MatlabExporter() {
		// TODO Auto-generated constructor stub
	}

	MatlabExporter::MatlabExporter(const char *filepath) {

		// check if file exists
		ifstream infile(filepath);
		if (!infile.good()) {
			ofstream data;
			data.open(filepath);
			data.close();
		} else {
			BOOST_LOG_TRIVIAL(fatal) << "file " << filepath << " already exists! Exiting.";
			std::exit(0);
		}
	}

	void MatlabExporter::dump(const char *filepath, list<Data> dataset) {

		ofstream data;
		data.open(filepath, fstream::app);
		while (!dataset.empty()) {
			data << std::fixed << std::setprecision(1) << dataset.front().rayNumber << ","
				<< std::setprecision(2) << dataset.front().x << ","
				<< std::setprecision(2) << dataset.front().y << ","
				<< std::setprecision(2) << dataset.front().z << ","
				<< std::setprecision(3) << dataset.front().omega_p << ","
				<< std::setprecision(4) << dataset.front().n_e << ","
				<< std::setprecision(6) << dataset.front().mu_r_sqrt << ","
				<< std::setprecision(4) << dataset.front().theta_0 << ","
				<< std::setprecision(1) << dataset.front().frequency << ","
				<< std::setprecision(12) << dataset.front().signalPower << ","
				<< std::setprecision(10) << dataset.front().timeOfFlight << ","
				<< std::setprecision(1) << dataset.front().collisionType << ","
				<< std::setprecision(1) << dataset.front().beaconId << ","
				<< std::setprecision(4) << dataset.front().azimuth_0 << "\n";
			dataset.pop_front();
		}
		data.close();

	}

} /* namespace exporter */
} /* namespace raytracer */
