//============================================================================
// Name        : JsonExporter.h
// Author      : Rian van Gijlswijk
// Description : Exports a 2D dataset to json format with data efficiently
//				 stored, to keep filesize low
//============================================================================

#ifndef EXPORTER_JSONEXPORTER_H_
#define EXPORTER_JSONEXPORTER_H_

#include "IExporter.h"

namespace raytracer {
namespace exporter {

class JsonExporter : public IExporter {

	public:
		JsonExporter();
		JsonExporter(const char *filepath);
		~JsonExporter() {}
		void dump(const char *filepath, list<Data> dataset);

};

} /* namespace exporter */
} /* namespace raytracer */

#endif /* EXPORTER_JSONEXPORTER_H_ */
