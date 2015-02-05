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
	boost::mutex tracingIncMutex;

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
		for (double freq = 2e6; freq <= 6e6; freq += 1e6) {
			for (double theta = 50; theta <= 80; theta += 5) {
				Ray r;
				r.frequency = freq;
				r.o.y = 2 + 3.39e6;
				r.originalAngle = theta * Constants::PI / 180.0;
				r.setNormalAngle(r.originalAngle);

				//r.trace();
				Worker w;
				threadGroup.add_thread(w.start(r));
			}
		}

		threadGroup.join_all();

		stop();

		double t = tmr.elapsed();
		double tracingsPerSec = numTracings / t;
	    printf("Elapsed: %5.2f sec. %d tracings done. %5.2f tracings/sec", t, numTracings, tracingsPerSec);

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

		double R = 3.39e6; // Mars radius. Todo: move to config files

		for (double theta = 0; theta < 2*Constants::PI; theta += Constants::PI/180) {
			double nextTheta = theta + Constants::PI/180;

			for (int h = 80000; h <= 200000; h += 1000) {
				Ionosphere io = Ionosphere(Vector2d((R + h) * cos(theta), (R + h) * sin(theta)),
						Vector2d((R + h) * cos(nextTheta), (R + h) * sin(nextTheta)));

				scm.addToScene(io);
			}

			Terrain tr = Terrain(Vector2d(R*cos(theta), R*sin(theta)),
					Vector2d(R*cos(nextTheta), R*sin(nextTheta)));

			//cout <<  tr.getMesh().begin.x << "," << tr.getMesh().begin.y << "," << tr.getMesh().end.x << "," << tr.getMesh().end.y << endl;

			scm.addToScene(tr);
		}
	}

	void Application::addToDataset(Data dat) {

		datasetMutex.lock();
		dataSet.push_back(dat);
		datasetMutex.unlock();
	}

	void Application::incrementTracing() {

		tracingIncMutex.lock();
		numTracings++;
		tracingIncMutex.unlock();
	}

	SceneManager Application::getSceneManager() {

		return scm;
	}

} /* namespace core */
} /* namespace raytracer */
