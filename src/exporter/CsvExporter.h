//============================================================================
// Name        : CsvExporter.h
// Author      : Rian van Gijlswijk
// Description : Exports a 2D dataset to a .dat file for use in Csv
//============================================================================

#ifndef CsvEXPORTER_H_
#define CsvEXPORTER_H_

#include "IExporter.h"

namespace raytracer {
namespace exporter {

using namespace math;

class CsvExporter : public IExporter {

	public:
		CsvExporter();
		~CsvExporter() {}
		void dump(const char *filepath, list<Vector2f> dataset);

};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* CsvEXPORTER_H_ */
