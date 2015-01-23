//============================================================================
// Name        : MatlabExporter.h
// Author      : Rian van Gijlswijk
// Description : Exports a 2D dataset to a .dat file for use in Matlab
//============================================================================

#ifndef MATLABEXPORTER_H_
#define MATLABEXPORTER_H_

#include "IExporter.h"

namespace raytracer {
namespace exporter {

class MatlabExporter : public IExporter {

	public:
		MatlabExporter();
		~MatlabExporter() {}
		void dump(const char *filepath, list<Data> dataset);

};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* MATLABEXPORTER_H_ */
