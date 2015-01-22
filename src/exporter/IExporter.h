//============================================================================
// Name        : IExporter.h
// Author      : Rian van Gijlswijk
// Description : Interface for data export implementation
//============================================================================

#ifndef IEXPORTER_H_
#define IEXPORTER_H_

#include <list>
#include "../math/Vector2f.h"

namespace raytracer {
namespace exporter {

	using namespace std;
	using namespace math;

	class IExporter {

		public:
			IExporter() {}
			virtual ~IExporter() {}
			virtual void dump(const char *filepath, list<Vector2f> dataset) = 0;

	};

}
}

#endif
