//============================================================================
// Name        : IExporter.h
// Author      : Rian van Gijlswijk
// Description : Interface for data export implementation
//============================================================================

#ifndef IEXPORTER_H_
#define IEXPORTER_H_

#include <list>
#include "Data.h"

namespace raytracer {
namespace exporter {

	using namespace std;

	class IExporter {

		public:
			IExporter() {}
			virtual ~IExporter() {}
			virtual void dump(const char *filepath, list<Data> dataset) = 0;

	};

}
}

#endif
