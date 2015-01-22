//============================================================================
// Name        : SceneManager.h
// Author      : Rian van Gijlswijk
// Description : Keeps track of all objects in the scene and calculates
// 				 collisions between scene objects and rays
//============================================================================

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include "../math/Line2f.h"
#include "../tracer/Ray.h"
#include "../tracer/Intersection.h"

namespace raytracer {
namespace scene {

	using namespace tracer;

	class SceneManager {
		public:
			SceneManager();
			Intersection intersect(Ray &r, Line2f &rayLine);
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* SCENEMANAGER_H_ */
