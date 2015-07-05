//============================================================================
// Name        : namespace.h
// Author      : Rian van Gijlswijk
// Description : Forward declaration of tracer classes
//============================================================================

#ifndef NAMESPACE_H_
#define NAMESPACE_H_

namespace raytracer {
	namespace tracer {
		class Ray;
		class Intersection;
	}
	namespace scene {
		class Geometry;
	}
	namespace math {
		class Vector3d;
		class Matrix3d;
	}
}

#endif
