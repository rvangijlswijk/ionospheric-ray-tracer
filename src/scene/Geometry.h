//============================================================================
// Name        : Geometry.h
// Author      : Rian van Gijlswijk
// Description : Base class defining basic behaviour of scene objects
//============================================================================

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../tracer/Ray.h"
#include "../math/Line2f.h"
#include "../math/Vector2f.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Geometry {

		public:
			Geometry();
			Geometry(Line2f mesh);
			Geometry(Vector2f begin, Vector2f end);
			Ray interact(Ray &r);
			Line2f getMesh();
			enum object_type {
				ionosphere,
				terrain,
				none
			};
			object_type type;

		protected:
			Line2f mesh2d;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* GEOMETRY_H_ */
