//============================================================================
// Name        : SceneManager.h
// Author      : Rian van Gijlswijk
// Description : Keeps track of all objects in the scene and calculates
// 				 collisions between scene objects and rays
//============================================================================

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <list>
#include "../math/Line2d.h"
#include "../tracer/Ray.h"
#include "../tracer/Intersection.h"
#include "Geometry.h"

namespace raytracer {
namespace scene {

	using namespace std;
	using namespace tracer;

	class SceneManager {

		public:
			SceneManager();
			Intersection intersect(Ray &r, Line2d &rayLine);
			void addToScene(Geometry* obj);
			void removeAllFromScene();
			list<Geometry*> getScene();

		private:
			list<Geometry*> sceneObjects;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* SCENEMANAGER_H_ */
