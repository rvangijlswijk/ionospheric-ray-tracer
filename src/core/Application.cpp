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
#include "../math/Constants.h"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;
	using namespace math;

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
		r.o.y = 2;
		originalAngle = 60.0 * Constants::PI / 180.0;
		r.setSolarZenithAngle(originalAngle);
		r.previousRefractiveIndex = 1.0; //75.0 * Constants::PI / 180.0;

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

		Terrain tr = Terrain(Vector2f(0, 1), Vector2f(3e5, 1));

		for (int h=100000; h<250000; h+= 100) {
			scm.addToScene(Ionosphere(Vector2f(0, h), Vector2f(3e5, h)));
		}

		scm.addToScene(tr);
	}

	SceneManager Application::getSceneManager() {

		return scm;
	}

} /* namespace core */
} /* namespace raytracer */
