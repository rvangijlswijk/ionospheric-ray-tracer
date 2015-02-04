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
	boost::mutex datasetMutex;

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
		for (float freq = 5e6; freq <= 5e6; freq += 1e6) {
			for (float theta = 60; theta <= 60; theta += 10) {
				Ray r;
				r.frequency = freq;
				r.o.y = 2 + 3390e3;
				r.originalAngle = theta * Constants::PI / 180.0;
				r.setNormalAngle(r.originalAngle);
				r.previousRefractiveIndex = 1.0; //75.0 * Constants::PI / 180.0;

				Worker w;
				threadGroup.add_thread(w.start(r));
			}
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

		float R = 3390e3; // Mars radius. Todo: move to config files

		for (float theta = 0; theta < 2*Constants::PI; theta += Constants::PI/180) {
			float nextTheta = theta + Constants::PI/180;

			for (int h = 80000; h <= 200000; h += 1000) {
				Ionosphere io = Ionosphere(Vector2f((R + h) * cos(theta), (R + h) * sin(theta)),
						Vector2f((R + h) * cos(nextTheta), (R + h) * sin(nextTheta)));

				scm.addToScene(io);
			}

			Terrain tr = Terrain(Vector2f(R*cos(theta), R*sin(theta)),
					Vector2f(R*cos(nextTheta), R*sin(nextTheta)));

			//cout <<  tr.getMesh().begin.x << "," << tr.getMesh().begin.y << "," << tr.getMesh().end.x << "," << tr.getMesh().end.y << endl;

			scm.addToScene(tr);
		}
	}

	void Application::addToDataset(Data dat) {

		datasetMutex.lock();
		dataSet.push_back(dat);
		datasetMutex.unlock();
	}

	SceneManager Application::getSceneManager() {

		return scm;
	}

} /* namespace core */
} /* namespace raytracer */
