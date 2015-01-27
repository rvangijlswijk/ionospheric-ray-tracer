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
			void setMesh(Line2f mesh);
			enum object_type {
				none = 0,
				ionosphere = 1,
				terrain = 2
			};
			object_type type;

		protected:
			Line2f mesh2d;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* GEOMETRY_H_ */
