//============================================================================
// Name        : Application.cpp
// Author      : Rian van Gijlswijk
// Description : Main application file.
//============================================================================

#include "Application.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

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

	void Application::init(int argc, char * argv[]) {

		parseCommandLineArgs(argc, argv);
		start();
		run();
	}

	void Application::parseCommandLineArgs(int argc, char * argv[]) {


		for (int i = 0; i < argc; i++) {

			if (strcmp(argv[i], "-h") == 0) {
				std::cout 	<< "Ionospheric Ray Tracer\n\n"
							<< "Synopsis:\n"
							<< "\tirt [-opts] scenarioConfig\n\n"
							<< "Description: \n"
							<< "\tPerform ionospheric ray tracing on a celestial object described by the _celestialConfig json file. "
							<< "If no config file is supplied, use a default scenario.\n\n"
							<< "Options:\n"
							<< "\t-c | --config\t Application config file\n"
							<< "\t-i | --iterations\t The number of consecutive times every ray option should be run.\n"
							<< "\t-h | --help\t This help.\n"
							<< "\t-p | --parallelism\t Multithreading indicator.\n"
							<< "\t-v | --verbose\t Verbose, display log output\n"
							<< "\t-vv \t\t Very verbose, display log and debug output\n";
				std::exit(0);

			} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--scenario") == 0) {
				_celestialConfigFile = argv[i+1];

			} else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
				_applicationConfigFile = argv[i+1];

			} else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--parallelism") == 0) {
				_parallelism = atoi(argv[i+1]);

			} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) {
				_iterations = atoi(argv[i+1]);

			} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
				_verbosity = boost::log::trivial::info;

			} else if (strcmp(argv[i], "-vv") == 0) {
				_verbosity = boost::log::trivial::debug;
			}
		}

		// load scenario config file. Must be given.
		_celestialConfigFile = argv[argc-1];
	}

	void Application::start() {

		_isRunning = true;

		_applicationConfig = Config(_applicationConfigFile);
		_celestialConfig = Config(_celestialConfigFile);

		if (_parallelism < 1) {
			_parallelism = _applicationConfig.getInt("parallelism");
		}
		if (_iterations < 1) {
			_iterations = _applicationConfig.getInt("iterations");
		}

//		boost::log::add_file_log("log/sample.log");

		boost::log::core::get()->set_filter(
				boost::log::trivial::severity >= _verbosity);

		tp = boost::threadpool::pool(_parallelism);

		BOOST_LOG_TRIVIAL(debug) << "applicationConfig: " << _applicationConfigFile << endl
				<< _applicationConfig << endl
				<< "celestialConfig:" << _celestialConfigFile << endl
				<< _celestialConfig;
	}

	void Application::run() {

		Timer tmr;

		BOOST_LOG_TRIVIAL(info) << "Parallelism is " << _applicationConfig.getInt("parallelism");
		BOOST_LOG_TRIVIAL(warning) << _applicationConfig.getInt("iterations") << " iterations";

		// trace a ray
		int rayCounter = 0;
		for (int iteration = 0; iteration < _applicationConfig.getInt("iterations"); iteration++) {

			BOOST_LOG_TRIVIAL(warning) << "Iteration " << (iteration+1) << " of " << _applicationConfig.getInt("iterations");

			createScene();

			int numWorkers = 0;
			double fmin = _applicationConfig.getObject("frequencies")["min"].asDouble();
			double fstep = _applicationConfig.getObject("frequencies")["step"].asDouble();
			double fmax = _applicationConfig.getObject("frequencies")["max"].asDouble();
			double SZAmin = _applicationConfig.getObject("SZA")["min"].asDouble();
			double SZAstep = _applicationConfig.getObject("SZA")["step"].asDouble();
			double SZAmax = _applicationConfig.getObject("SZA")["max"].asDouble();

			BOOST_LOG_TRIVIAL(info) << "Scanning frequencies " << fmin << " Hz to " << fmax << "Hz with steps of " << fstep << "Hz";
			BOOST_LOG_TRIVIAL(info) << "Scanning SZA " << SZAmin << " deg to " << SZAmax << " deg with steps of " << SZAstep << " deg";

			for (double freq = fmin; freq <= fmax; freq += fstep) {
				for (double SZA = SZAmin; SZA <= SZAmax; SZA += SZAstep) {
						Ray r;
						r.rayNumber = ++rayCounter;
						r.frequency = freq;
						r.signalPower = 0;
						r.o.y = 2 + _celestialConfig.getInt("radius");
						r.originalAngle = SZA * Constants::PI / 180.0;
						Vector3d direction = Vector3d(cos(Constants::PI/2.0 - r.originalAngle),
								sin(Constants::PI/2.0 - r.originalAngle),
								0);
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
		double tracingsPerSec = _numTracings / t;
	    printf("Elapsed: %5.2f sec. %d tracings done. %5.2f tracings/sec", t, _numTracings, tracingsPerSec);

		//CsvExporter ce;
		//ce.dump("Debug/data.csv", dataSet);
		MatlabExporter me;
		me.dump("Debug/data.dat", dataSet);
	}

	void Application::stop() {

		_isRunning = false;
	}

	/**
	 * Add geometries to the scenemanager
	 */
	void Application::createScene() {

		int numSceneObjectsCreated = 0;
		double R = _celestialConfig.getInt("radius");
		double angularStepSize = Constants::PI/360;
		IonosphereConfigParser plh = IonosphereConfigParser();

		// terrain
		for (double latitude = Constants::PI/2; latitude < Constants::PI/2 + 10*Constants::PI/180; latitude += angularStepSize) {
			for (double theta = 0; theta < Constants::PI/2; theta += angularStepSize) {

				Vector3d N = Vector3d(cos(theta), sin(theta), cos(latitude)).norm();
				Plane3d mesh = Plane3d(N, Vector3d(R*N.x, R*N.y, R*N.z));
				mesh.size = angularStepSize * R;
				Terrain* tr = new Terrain(mesh);

				numSceneObjectsCreated++;
				_scm.addToScene(tr);
			}
		}

		const Json::Value ionosphereConfig = _celestialConfig.getObject("ionosphere");
		int start = ionosphereConfig["start"].asInt();
		int dh = ionosphereConfig["step"].asInt();
		int end =ionosphereConfig["end"].asInt();

		for (double latitude = Constants::PI/2; latitude < Constants::PI/2 + 10*Constants::PI/180; latitude += angularStepSize) {
			for (double theta = Constants::PI/4; theta < Constants::PI/2; theta += angularStepSize) {
				for (int h = start; h < end; h += dh) {

					Vector3d N = Vector3d(cos(theta), sin(theta), cos(latitude)).norm();
					Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
					mesh.size = angularStepSize * R;
					Ionosphere* io = new Ionosphere(mesh);
					io->layerHeight = dh;

					for (int idx = 0; idx < ionosphereConfig["layers"].size(); idx++) {

						double electronPeakDensity = atof(ionosphereConfig["layers"][idx].get("electronPeakDensity", "").asCString());
						double peakProductionAltitude = ionosphereConfig["layers"][idx].get("peakProductionAltitude", "").asDouble();
						double neutralScaleHeight = ionosphereConfig["layers"][idx].get("neutralScaleHeight", 11.1e3).asDouble();

						io->superimposeElectronNumberDensity(electronPeakDensity, peakProductionAltitude, neutralScaleHeight);
					}

					numSceneObjectsCreated++;
					_scm.addToScene(io);
				}
			}
		}

//		const Json::Value atmosphereConfig = _celestialConfig.getObject("atmosphere");
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
//				_scm.addToScene(atm);
//			}
//		}

		if (numSceneObjectsCreated > 1e9)
			BOOST_LOG_TRIVIAL(info) << setprecision(3) << numSceneObjectsCreated/1.0e9 << "G scene objects created";
		else if (numSceneObjectsCreated > 1e6)
			BOOST_LOG_TRIVIAL(info) << setprecision(3) << numSceneObjectsCreated/1.0e6 << "M scene objects created";
		else if (numSceneObjectsCreated > 1e3)
			BOOST_LOG_TRIVIAL(info) << setprecision(3) << numSceneObjectsCreated/1.0e3 << "K scene objects created";
		else
			BOOST_LOG_TRIVIAL(info) << setprecision(3) << numSceneObjectsCreated << " scene objects created";
	}

	/**
	 * Flush the scene by clearing the list of scene objects
	 */
	void Application::flushScene() {

		_scm.removeAllFromScene();
	}

	void Application::addToDataset(Data dat) {

		datasetMutex.lock();
		dataSet.push_back(dat);
		datasetMutex.unlock();
	}

	void Application::incrementTracing() {

		tracingIncMutex.lock();
		_numTracings++;
		tracingIncMutex.unlock();
	}

	SceneManager Application::getSceneManager() {

		return _scm;
	}

	Config Application::getApplicationConfig() {

		return _applicationConfig;
	}

	Config Application::getCelestialConfig() {

		return _celestialConfig;
	}

	void Application::setCelestialConfig(Config conf) {

		_celestialConfig = conf;
	}

} /* namespace core */
} /* namespace raytracer */
