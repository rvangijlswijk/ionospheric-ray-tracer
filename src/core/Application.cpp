//============================================================================
// Name        : Application.cpp
// Author      : Rian van Gijlswijk
// Description : Main application file.
//============================================================================

#include <iostream>
#include "Application.h"
#include "../tracer/Ray.h"
#include "../exporter/CsvExporter.h"
#include "../exporter/MatlabExporter.h"
#include "../scene/Ionosphere.h"
#include "../scene/Terrain.h"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;

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
		r.o = Vector2f(1,1);
		r.d = Vector2f(1,1.7320);

		r.trace();

		stop();

		//CsvExporter ce;
		//ce.dump("Debug/data.csv", rayPath);
		MatlabExporter me;
		me.dump("Debug/data.dat", dataSet);
	}

	void Application::stop() {

		isRunning = false;
	}

	/**
	 * Add geometries to the scenemanager
	 */
	void Application::createScene() {

		Terrain tr = Terrain(Vector2f(0, 1), Vector2f(150000, 1));

		for (int h=90000; h<140000; h+= 1000) {
			scm.addToScene(Ionosphere(Vector2f(0, h), Vector2f(150000, h)));
		}

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
