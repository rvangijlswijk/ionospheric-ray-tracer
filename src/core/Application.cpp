//============================================================================
// Name        : Application.cpp
// Author      : Rian van Gijlswijk
// Description : Main application file.
//============================================================================

#include <iostream>
#include <list>
#include "Application.h"
#include "../tracer/Ray.h"
#include "../exporter/MatlabExporter.h"
#include "../scene/Ionosphere.h"
#include "../scene/Terrain.h"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;

	list<Vector2f> rayPath;

	void Application::init() {

		createScene();
		start();
		run();
	}

	void Application::start() {

		isRunning = true;
	}

	void Application::run() {

		// trace a ray
		Ray r;
		r.o = Vector2f(0,0);
		r.d = Vector2f(1,1.94);

		r.trace(rayPath);

		MatlabExporter me;
		me.dump("data.dat", rayPath);

		stop();
	}

	void Application::stop() {

		isRunning = false;
	}

	/**
	 * Add geometries to the scenemanager
	 */
	void Application::createScene() {

		Ionosphere io = Ionosphere(Vector2f(0, 100), Vector2f(150, 100));
		Terrain tr = Terrain(Vector2f(0, 1), Vector2f(150, 1));

		scm.addToScene(io);
		scm.addToScene(tr);
	}

	SceneManager Application::getSceneManager() {

		return scm;
	}

} /* namespace core */
} /* namespace raytracer */


int main() {

	raytracer::core::Application::getInstance().init();

}
