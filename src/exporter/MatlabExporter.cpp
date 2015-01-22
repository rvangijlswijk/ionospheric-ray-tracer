/*
 * DatExporter.cpp
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
using namespace math;

MatlabExporter::MatlabExporter() {
	// TODO Auto-generated constructor stub
}

void MatlabExporter::dump(const char *filepath, list<Vector2f> dataset) {

	ofstream data;
	data.open(filepath);
	while (!dataset.empty()) {
		data << dataset.front().x << "," << dataset.front().y << "\n";
		dataset.pop_front();
	}
	data.close();

}

} /* namespace exporter */
} /* namespace raytracer */
