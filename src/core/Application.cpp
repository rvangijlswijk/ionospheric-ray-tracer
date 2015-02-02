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
#include "../threading/Worker.h"
#include "Timer.cpp"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;
	using namespace math;
	using namespace threading;

	boost::thread_group threadGroup;

	void Application::init() {

		createScene();
		start();
		run();
	}

	void Application::start() {

		isRunning = true;
	}

	void Application::run() {

		Timer tmr;

		// trace a ray
		for (float theta = 15; theta < 90; theta += 15) {
			Ray r;
			r.o.y = 2;
			r.originalAngle = theta * Constants::PI / 180.0;
			r.setSolarZenithAngle(r.originalAngle);
			r.previousRefractiveIndex = 1.0; //75.0 * Constants::PI / 180.0;

			rays.push_back(r);
		}

		while (rays.size() > 0) {
			Ray r = rays.front();
			cout << "Simulating for theta:" << r.originalAngle << "\n";
			Worker w;
			threadGroup.add_thread(w.start(r));
			rays.pop_front();
		}

		threadGroup.join_all();

		stop();

		double t = tmr.elapsed();
	    cout << "Elapsed:" << t << endl;

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

		for (int h=80000; h<250000; h+= 10) {
			scm.addToScene(Ionosphere(Vector2f(0, h), Vector2f(3e5, h)));
		}

		scm.addToScene(tr);
	}

	SceneManager Application::getSceneManager() {

		return scm;
	}

} /* namespace core */
} /* namespace raytracer */
