/*
 * JsonExporter.cpp
 *
 *  Created on: 8 Jul 2015
 *      Author: rian
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include "JsonExporter.h"

namespace raytracer {
namespace exporter {

	using namespace std;

	JsonExporter::JsonExporter() {
		// TODO Auto-generated constructor stub
	}

	JsonExporter::JsonExporter(const char *filepath) {

		ofstream data;
		data.open(filepath);
		data.close();
	}

	/**
	 * Json file format:
	 * {
	 * 	rays: [{
	 * 		header: {
	 * 			omega_p: ...,
	 * 			theta_0: ...,
	 * 			freq: ...,
	 * 			bId: ...,
	 * 			azimuth_0: ...
	 * 		},
	 * 		data: [
	 * 			x,y,z,n_e,mu_r_sqrt,signalPower,timeOfFlight,collisionType
	 * 		]
	 * 	},{...}]
	 * }
	 */
	void JsonExporter::dump(const char *filepath, list<Data> dataset) {

		ofstream datafile;
		datafile.open(filepath, fstream::app);

		// construct beginning of file
		datafile 	<< "{\n"
					<< "\"rays\": [\n";

		// file body
		int rayInProcess = 0;
		while (!dataset.empty()) {
			Data item = dataset.front();

			// generate header for this item
			if (item.rayNumber > rayInProcess) {
				if (rayInProcess == 0) {
					datafile << "{";
				} else {
					datafile << "]]},{";
				}
				rayInProcess = item.rayNumber;
				datafile 	<< "\n\"header\": {\n"
						<< "\"o_p\": " << std::setprecision(3) << item.omega_p << ",\n"
						<< "\"t_0\": " << std::setprecision(4) << item.theta_0 << ",\n"
						<< "\"f\": " << std::setprecision(1) << item.frequency << ",\n"
						<< "\"bId\": " << std::setprecision(1) << item.beaconId << ",\n"
						<< "\"a_0\": " << std::setprecision(4) << item.azimuth_0 << "\n"
						<< "},\n\"data\":[[\n";
			} else {
				datafile << "],[";
			}

			// generate body for this item
			datafile << std::fixed << std::setprecision(2) << item.x << ",\n"
				<< std::setprecision(2) << item.y << ",\n"
				<< std::setprecision(2) << item.z << ",\n"
				<< std::setprecision(4) << item.n_e << ",\n"
				<< std::setprecision(6) << item.mu_r_sqrt << ",\n"
				<< std::setprecision(12) << item.signalPower << ",\n"
				<< std::setprecision(10) << item.timeOfFlight << ",\n"
				<< std::setprecision(1) << item.collisionType << "\n";
			dataset.pop_front();
		}

		// construct end of file
		datafile	<< "]]}]\n}";

		datafile.close();
	}

} /* namespace exporter */
} /* namespace raytracer */
