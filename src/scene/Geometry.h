//============================================================================
// Name        : Geometry.h
// Author      : Rian van Gijlswijk
// Description : Base class defining basic behaviour of scene objects
//============================================================================

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "../tracer/Ray.h"
#include "../math/Line2d.h"
#include "../math/Vector2d.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class Geometry {

		public:
			Geometry();
			Geometry(Line2d mesh);
			Geometry(Vector2d begin, Vector2d end);
			virtual void interact(Ray *r, Vector2d &hitpos);
			Line2d getMesh();
			void setMesh(Line2d mesh);
			double getSolarZenithAngle2d();
			enum object_type {
				none = 0,
				ionosphere = 1,
				terrain = 2
			};
			object_type type = none;
			Line2d mesh2d;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* GEOMETRY_H_ */
