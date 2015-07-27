//============================================================================
// Name        : Application.cpp
// Author      : Rian van Gijlswijk
// Description : Main application file.
//============================================================================

#include "Application.h"
#include <string>
#include <regex>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include "Timer.cpp"
#include "CommandLine.h"
#include "../math/Matrix3d.h"
#include "../exporter/JsonExporter.h"
#include "../exporter/MatlabExporter.h"
#include "../radio/AntennaFactory.h"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;
	using namespace math;
	using namespace threading;
	using namespace radio;

	boost::mutex datasetMutex;
	boost::mutex tracingIncMutex;

	boost::threadpool::pool tp;

	void Application::init(int argc, char * argv[]) {

		BOOST_LOG_TRIVIAL(debug) << "Init application";

		AntennaFactory::printMappedTypes();

		parseCommandLineArgs(argc, argv);
		start();
		run();
	}

	void Application::parseCommandLineArgs(int argc, char * argv[]) {

		BOOST_LOG_TRIVIAL(debug) << "parseCommandLineArgs";

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
							<< "\t-o | --output\t Path where output file should be stored.\n"
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

			} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				_outputFile = argv[i+1];

			} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) {
				_iterations = atoi(argv[i+1]);

			} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
				_verbosity = boost::log::trivial::info;

			} else if (strcmp(argv[i], "-vv") == 0) {
				_verbosity = boost::log::trivial::debug;
			}
		}

		// load scenario config file. Must be given.
		if (!std::regex_match (argv[argc-1], std::regex("[A-Za-z0-9_/]+\.json") )) {
			BOOST_LOG_TRIVIAL(fatal) << "No scenario file given! Exiting.";
			std::exit(0);
		}
		_celestialConfigFile = argv[argc-1];
	}

	void Application::start() {

		BOOST_LOG_TRIVIAL(debug) << "Start application";

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

		_me = new MatlabExporter(_outputFile);
	}

	void Application::run() {

		BOOST_LOG_TRIVIAL(debug) << "Run application";

		Timer tmr;
		int radius = _celestialConfig.getInt("radius");

		BOOST_LOG_TRIVIAL(info) << "Parallelism is " << _applicationConfig.getInt("parallelism");
		if (_verbosity > boost::log::trivial::info) {
			std::ostringstream stringStream;
			stringStream << "Parallelism is " << _applicationConfig.getInt("parallelism");
			CommandLine::getInstance().addToHeader(stringStream.str().c_str());
		}
		BOOST_LOG_TRIVIAL(info) << _applicationConfig.getInt("iterations") << " iterations";

		// load config values
		double fmin = _applicationConfig.getObject("frequencies")["min"].asDouble();
		double fstep = _applicationConfig.getObject("frequencies")["step"].asDouble();
		double fmax = _applicationConfig.getObject("frequencies")["max"].asDouble();
		double SZAmin = _applicationConfig.getObject("SZA")["min"].asDouble();
		double SZAstep = _applicationConfig.getObject("SZA")["step"].asDouble();
		double SZAmax = _applicationConfig.getObject("SZA")["max"].asDouble();
		double azimuthMin = _applicationConfig.getObject("azimuth")["min"].asDouble();
		double azimuthStep = _applicationConfig.getObject("azimuth")["step"].asDouble();
		double azimuthMax = _applicationConfig.getObject("azimuth")["max"].asDouble();
		const Json::Value beacons = _applicationConfig.getArray("beacons");

		// trace a ray
		int rayCounter = 0;
		for (int iteration = 0; iteration < _applicationConfig.getInt("iterations"); iteration++) {

			BOOST_LOG_TRIVIAL(info) << "Iteration " << (iteration+1) << " of " << _applicationConfig.getInt("iterations");

			createScene();

			BOOST_LOG_TRIVIAL(info) << "Simulating " << beacons.size() << " beacons";
			BOOST_LOG_TRIVIAL(info) << "Scanning frequencies " << fmin << " Hz to " << fmax << "Hz with steps of " << fstep << "Hz";
			BOOST_LOG_TRIVIAL(info) << "Scanning SZA 9" << SZAmin << " deg to " << SZAmax << " deg with steps of " << SZAstep << " deg";
			BOOST_LOG_TRIVIAL(info) << "Scanning azimuth " << azimuthMin << " deg to " << azimuthMax << " deg with steps of " << azimuthStep << " deg";

			if (_verbosity > boost::log::trivial::info) {
				std::ostringstream stringStream;
				stringStream << "Simulating " << beacons.size() << " beacons\n" << "Scanning frequencies " << fmin << " Hz to " << fmax
						<< "Hz with steps of " << fstep << "Hz\n" << "Scanning SZA " << SZAmin << " deg to "
						<< SZAmax << " deg with steps of " << SZAstep << " deg\n"
						<< "Scanning azimuth " << azimuthMin << " deg to " << azimuthMax << " deg with steps of " << azimuthStep << " deg";
						CommandLine::getInstance().addToHeader(stringStream.str().c_str());
			}

			for(int b = 0; b < beacons.size(); b++) {

				double latitudeOffset = beacons[b].get("latitudeOffset", "").asDouble() * Constants::PI / 180.0;
				double longitudeOffset = beacons[b].get("longitudeOffset", "").asDouble() * Constants::PI / 180.0;
				const Json::Value antenna = beacons[b].get("antenna", "");
				IAntenna* ant = AntennaFactory::createInstance(antenna.get("type", "").asString());
				ant->setConfig(antenna);

				Matrix3d latitude = Matrix3d::createRotationMatrix(latitudeOffset, Matrix3d::ROTATION_X);
				Matrix3d longitude = Matrix3d::createRotationMatrix(longitudeOffset, Matrix3d::ROTATION_Z);
				Matrix3d rotationMatrix = latitude * longitude;

				Vector3d startPosition = rotationMatrix * Vector3d(0, (radius+2), 0);
				BOOST_LOG_TRIVIAL(debug) << startPosition;

				for(double azimuth = azimuthMin; azimuth <= azimuthMax; azimuth += azimuthStep) {

					Matrix3d azimuthRotation = Matrix3d::createRotationMatrix(azimuth * Constants::PI / 180, Matrix3d::ROTATION_Y);

					for (double freq = fmin; freq <= fmax; freq += fstep) {
						for (double elevation = SZAmin; elevation <= SZAmax; elevation += SZAstep) {

							Ray r;
							r.rayNumber = ++rayCounter;
							r.frequency = freq;
							r.signalPower = ant->getSignalPowerAt(azimuth, elevation);
							r.o = startPosition;
							r.originalAngle = elevation * Constants::PI / 180.0;
							r.originBeaconId = b+1;
							r.originalAzimuth = azimuth * Constants::PI / 180.0;
							Vector3d direction = Vector3d(cos(Constants::PI/2.0 - r.originalAngle),
									sin(Constants::PI/2.0 - r.originalAngle),
									0).norm();
							r.d = azimuthRotation * direction;

							Worker w;
							w.schedule(&tp, r);

							numWorkers++;
						}
					}
				}
			}

			BOOST_LOG_TRIVIAL(info) << numWorkers << " workers queued";
			if (_verbosity > boost::log::trivial::info) {
				std::ostringstream stringStream;
				stringStream << numWorkers << " workers queued";
				CommandLine::getInstance().addToHeader(stringStream.str().c_str());
			}

			tp.wait();

			flushScene();
		}

		stop();

		double t = tmr.elapsed();
		double tracingsPerSec = _numTracings / t;
		char buffer[80];
		CommandLine::getInstance().updateBody("\n");
	    sprintf(buffer, "Elapsed: %5.2f sec. %d tracings done. %5.2f tracings/sec",
	    		t, _numTracings, tracingsPerSec);
	    BOOST_LOG_TRIVIAL(warning) << buffer;

		//CsvExporter ce;
		//ce.dump("Debug/data.csv", dataSet);
		_me->dump(_outputFile, dataSet);

	    BOOST_LOG_TRIVIAL(warning) << "Results stored at: " << _outputFile;
	}

	void Application::stop() {

		_isRunning = false;
	}

	/**
	 * Add geometries to the scenemanager
	 */
	void Application::createScene() {

		_scm = SceneManager();
		_scm.loadStaticEnvironment();

		int numSceneObjectsCreated = 0;
		double R = _celestialConfig.getInt("radius");
		double angularStepSize = _applicationConfig.getDouble("angularStepSize");

		for (double latitude = -30 * Constants::PI / 180.0; latitude <= 30 * Constants::PI / 180.0; latitude += angularStepSize) {
			for (double longitude = -30 * Constants::PI / 180.0; longitude <= 30 * Constants::PI / 180.0; longitude += angularStepSize) {

				Matrix3d latitudeM = Matrix3d::createRotationMatrix(latitude, Matrix3d::ROTATION_X);
				Matrix3d longitudeM = Matrix3d::createRotationMatrix(longitude, Matrix3d::ROTATION_Z);
				Matrix3d rotationMatrix = latitudeM * longitudeM;

				Vector3d startPosition = rotationMatrix * Vector3d(0, R, 0);

				Plane3d mesh = Plane3d(startPosition.norm(), startPosition);
				mesh.size = angularStepSize * R;
				Terrain* tr = new Terrain(mesh);

				numSceneObjectsCreated++;
				_scm.addToScene(tr);
			}
		}

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
		if (dataSet.size() > Data::MAX_DATASET_SIZE) {
			_me->dump(_outputFile, dataSet);
			dataSet.clear();
		}
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

	void Application::setApplicationConfig(Config conf) {

		_applicationConfig = conf;
	}

	int Application::getVerbosity() {

		return _verbosity;
	}

} /* namespace core */
} /* namespace raytracer */
