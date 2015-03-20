//============================================================================
// Name        : Application.cpp
// Author      : Rian van Gijlswijk
// Description : Main application file.
//============================================================================

#include "Application.h"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;
	using namespace math;
	using namespace threading;

	boost::mutex datasetMutex;
	boost::mutex tracingIncMutex;

	boost::threadpool::pool tp(4);

	void Application::init() {

		start();
		run();
	}

	void Application::start() {

		isRunning = true;
		Config::getInstance().loadFromFile("config/mars.json");

		boost::log::core::get()->set_filter(
				boost::log::trivial::severity >= boost::log::trivial::info);
	}

	void Application::run() {

		Timer tmr;

		// trace a ray
		int rayCounter = 0;
		for (int iteration = 0; iteration < 1; iteration++) {

			BOOST_LOG_TRIVIAL(info) << "Iteration " << iteration;

			createScene();

			for (double freq = 5e6; freq <= 5e6; freq += 0.5e6) {
				for (double SZA = 10; SZA <= 80; SZA += 10) {
					Ray r;
					r.rayNumber = ++rayCounter;
					r.frequency = freq;
					r.signalPower = 0;
					r.o.y = 2 + Config::getInstance().getInt("radius");
					r.originalAngle = SZA * Constants::PI / 180.0;
					r.setNormalAngle(r.originalAngle);

					Worker w;
					w.schedule(&tp, r);
				}
			}

			BOOST_LOG_TRIVIAL(info) << (tp.pending() + tp.size()) << " workers queued";

			tp.wait();

			flushScene();
		}

		stop();

		double t = tmr.elapsed();
		double tracingsPerSec = numTracings / t;
	    printf("Elapsed: %5.2f sec. %d tracings done. %5.2f tracings/sec", t, numTracings, tracingsPerSec);

		//CsvExporter ce;
		//ce.dump("Debug/data.csv", dataSet);
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

		double R = Config::getInstance().getInt("radius");

		// terrain
		for (double theta = 0; theta < 2*Constants::PI; theta += Constants::PI/180) {
			double nextTheta = theta + Constants::PI/180;

			Terrain* tr = new Terrain(Vector2d(R*cos(theta), R*sin(theta)),
					Vector2d(R*cos(nextTheta), R*sin(nextTheta)));

			scm.addToScene(tr);
		}

		int dh = 500;
		const Json::Value ionosphereConfig = Config::getInstance().getArray("ionosphere");
		for (int idx = 0; idx < ionosphereConfig.size(); idx++) {

			int hS = ionosphereConfig[idx].get("start", 0).asInt();
			int hE = ionosphereConfig[idx].get("end", 0).asInt();
			double electronPeakDensity = atof(ionosphereConfig[idx].get("electronPeakDensity", "").asCString());
			double peakProductionAltitude = ionosphereConfig[idx].get("peakProductionAltitude", "").asDouble();
			for (double theta = 0; theta < 2*Constants::PI; theta += Constants::PI/180) {
				double nextTheta = theta + Constants::PI/180;

				for (int h = hS; h < hE; h += dh) {
					Ionosphere* io = new Ionosphere(Vector2d((R + h) * cos(theta), (R + h) * sin(theta)),
							Vector2d((R + h) * cos(nextTheta), (R + h) * sin(nextTheta)));
					io->layerHeight = dh;
					io->electronPeakDensity = electronPeakDensity;
					io->peakProductionAltitude = peakProductionAltitude;

					scm.addToScene(io);
				}
			}
		}

		const Json::Value atmosphereConfig = Config::getInstance().getObject("atmosphere");
		int hS = atmosphereConfig.get("start", 0).asInt();
		int hE = atmosphereConfig.get("end", 0).asInt();

		for (double theta = 0; theta < 2*Constants::PI; theta += Constants::PI/180) {
			double nextTheta = theta + Constants::PI/180;

			for (int h = hS; h < hE; h += dh) {
				Atmosphere* atm = new Atmosphere(Vector2d((R + h) * cos(theta), (R + h) * sin(theta)),
						Vector2d((R + h) * cos(nextTheta), (R + h) * sin(nextTheta)));
				atm->layerHeight = dh;

				scm.addToScene(atm);
			}
		}
	}

	/**
	 * Flush the scene by clearing the list of scene objects
	 */
	void Application::flushScene() {

		scm.removeAllFromScene();
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
