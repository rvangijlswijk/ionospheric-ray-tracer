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

	boost::threadpool::pool tp;

	void Application::init() {

		start();
		run();
	}

	void Application::start() {

		isRunning = true;
		applicationConfig = Config("config/config.json");
		celestialConfig = Config("config/scenario_default.json");

		boost::log::core::get()->set_filter(
				boost::log::trivial::severity >= boost::log::trivial::info);

		tp = boost::threadpool::pool(applicationConfig.getInt("parallelism"));
	}

	void Application::run() {

		Timer tmr;

		BOOST_LOG_TRIVIAL(info) << "Parallelism is " << applicationConfig.getInt("parallelism");
		BOOST_LOG_TRIVIAL(info) << applicationConfig.getInt("iterations") << " iterations";

		// trace a ray
		int rayCounter = 0;
		for (int iteration = 0; iteration < applicationConfig.getInt("iterations"); iteration++) {

			BOOST_LOG_TRIVIAL(info) << "Iteration " << (iteration+1) << " of " << applicationConfig.getInt("iterations");

			createScene();

			int numWorkers = 0;
			for (double freq = 4.5e6; freq <= 5e6; freq += 0.5e6) {
				for (double SZA = 10; SZA <= 80; SZA += 10) {
					Ray r;
					r.rayNumber = ++rayCounter;
					r.frequency = freq;
					r.signalPower = 0;
					r.o.y = 2 + celestialConfig.getInt("radius");
					r.originalAngle = SZA * Constants::PI / 180.0;
					r.setNormalAngle(r.originalAngle);

					Worker w;
					w.schedule(&tp, r);

					numWorkers++;
				}
			}

			BOOST_LOG_TRIVIAL(info) << numWorkers << " workers queued";

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

		double R = celestialConfig.getInt("radius");
		double angularStepSize = Constants::PI/180;
		IonosphereConfigParser plh = IonosphereConfigParser();

		// terrain
		for (double theta = 0; theta < 2*Constants::PI; theta += angularStepSize) {

			Terrain* tr = new Terrain(Vector3d(cos(theta), sin(theta), 0), Vector3d(R*cos(theta), R*sin(theta), 0));

			scm.addToScene(tr);
		}

		int dh = 500;
		const Json::Value ionosphereConfig = celestialConfig.getArray("ionosphere");
		for (int idx = 0; idx < ionosphereConfig.size(); idx++) {

			int hS = ionosphereConfig[idx].get("start", 0).asInt();
			int hE = ionosphereConfig[idx].get("end", 0).asInt();
			double electronPeakDensity = atof(ionosphereConfig[idx].get("electronPeakDensity", "").asCString());
			double peakProductionAltitude = ionosphereConfig[idx].get("peakProductionAltitude", "").asDouble();
			Json::Value stratificationRaw = ionosphereConfig[idx].get("stratification", "");
			const char * stratificationType = stratificationRaw.asCString();
			for (double theta = 0; theta < 2*Constants::PI; theta += angularStepSize) {

				for (int h = hS; h < hE; h += dh) {
					Plane3d mesh = Plane3d(Vector3d(cos(theta), sin(theta), 0), Vector3d((R + h) * cos(theta), (R + h) * sin(theta), 0));
					mesh.size = angularStepSize * R;
					Ionosphere* io = new Ionosphere(mesh);
					io->layerHeight = dh;
					io->setElectronPeakDensity(electronPeakDensity);
					io->setPeakProductionAltitude(peakProductionAltitude);

					scm.addToScene(io);

					dh = plh.getDh(stratificationType, h);
				}
			}
		}

//		const Json::Value atmosphereConfig = celestialConfig.getObject("atmosphere");
//		int hS = atmosphereConfig.get("start", 0).asInt();
//		int hE = atmosphereConfig.get("end", 0).asInt();
//		dh = 2000;
//
//		for (double theta = 0; theta < 2*Constants::PI; theta += Constants::PI/180) {
//			double nextTheta = theta + Constants::PI/180;
//
//			for (int h = hS; h < hE; h += dh) {
//				Atmosphere* atm = new Atmosphere(Vector3d(cos(theta), sin(theta), 0),Vector3d((R + h) * cos(theta), (R + h) * sin(theta), 0));
//				atm->layerHeight = dh;
//
//				scm.addToScene(atm);
//			}
//		}
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

	Config Application::getApplicationConfig() {

		return applicationConfig;
	}

	Config Application::getCelestialConfig() {

		return celestialConfig;
	}

	void Application::setCelestialConfig(Config conf) {

		celestialConfig = conf;
	}

} /* namespace core */
} /* namespace raytracer */
