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

//		boost::log::add_file_log("log/sample.log");

		boost::log::core::get()->set_filter(
				boost::log::trivial::severity >= boost::log::trivial::warning);

		tp = boost::threadpool::pool(applicationConfig.getInt("parallelism"));
	}

	void Application::run() {

		Timer tmr;

		BOOST_LOG_TRIVIAL(warning) << "Parallelism is " << applicationConfig.getInt("parallelism");
		BOOST_LOG_TRIVIAL(warning) << applicationConfig.getInt("iterations") << " iterations";

		// trace a ray
		int rayCounter = 0;
		for (int iteration = 0; iteration < applicationConfig.getInt("iterations"); iteration++) {

			BOOST_LOG_TRIVIAL(warning) << "Iteration " << (iteration+1) << " of " << applicationConfig.getInt("iterations");

			createScene();

			int numWorkers = 0;
			for (double freq = 5e6; freq <= 5e6; freq += 0.5e6) {
				for (double SZA = 10; SZA <= 80; SZA += 10) {
					Ray r;
					r.rayNumber = ++rayCounter;
					r.frequency = freq;
					r.signalPower = 0;
					r.o.y = 2 + celestialConfig.getInt("radius");
					r.originalAngle = SZA * Constants::PI / 180.0;
					Vector3d direction = Vector3d(cos(Constants::PI/2.0 - r.originalAngle),
							sin(Constants::PI/2.0 - r.originalAngle),
							tan(-4 * Constants::PI / 180));
					r.d = direction.norm();

					Worker w;
					w.schedule(&tp, r);

					numWorkers++;
				}
			}

			BOOST_LOG_TRIVIAL(warning) << numWorkers << " workers queued";

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

		int numSceneObjectsCreated = 0;
		double R = celestialConfig.getInt("radius");
		double angularStepSize = Constants::PI/360;
		IonosphereConfigParser plh = IonosphereConfigParser();

		// terrain
		for (double latitude = Constants::PI/2; latitude < Constants::PI/2 + 10*Constants::PI/180; latitude += angularStepSize) {
			for (double theta = Constants::PI/4; theta < Constants::PI/2; theta += angularStepSize) {

				Vector3d N = Vector3d(cos(theta), sin(theta), cos(latitude)).norm();
				Plane3d mesh = Plane3d(N, Vector3d(R*N.x, R*N.y, R*N.z));
				mesh.size = angularStepSize * R;
				Terrain* tr = new Terrain(mesh);

				numSceneObjectsCreated++;
				scm.addToScene(tr);
			}
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
			for (double latitude = Constants::PI/2; latitude < Constants::PI/2 + 10*Constants::PI/180; latitude += angularStepSize) {
				for (double theta = Constants::PI/4; theta < Constants::PI/2; theta += angularStepSize) {

					for (int h = hS; h < hE; h += dh) {
						Vector3d N = Vector3d(cos(theta), sin(theta), cos(latitude)).norm();
						Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
						mesh.size = angularStepSize * R;
						Ionosphere* io = new Ionosphere(mesh);
						io->layerHeight = dh;
						io->setElectronPeakDensity(electronPeakDensity);
						io->setPeakProductionAltitude(peakProductionAltitude);

						numSceneObjectsCreated++;
						scm.addToScene(io);

						//dh = plh.getDh(stratificationType, h);
					}
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
//				numSceneObjectsCreated++;
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
