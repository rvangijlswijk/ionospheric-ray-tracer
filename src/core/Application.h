//============================================================================
// Name        : Application.h
// Author      : Rian van Gijlswijk
// Description : Main application file. Responsible for starting, running and
//				 stopping the simulation
//============================================================================

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <list>
#include <stdlib.h>
#include <boost/thread.hpp>
#include "Config.h"
#include "../scene/SceneManager.h"
#include "../scene/Ionosphere.h"
#include "../scene/Atmosphere.h"
#include "../scene/Terrain.h"
#include "../tracer/Ray.h"
#include "../exporter/Data.h"
#include "../exporter/CsvExporter.h"
#include "../exporter/MatlabExporter.h"
#include "../math/Constants.h"
#include "../math/NormalDistribution.h"
#include "../threading/Worker.h"
#include "../../contrib/threadpool/threadpool.hpp"
#include "../../contrib/jsoncpp/value.h"
#include "../scene/IonosphereConfigParser.h"

namespace raytracer {
namespace core {

	using namespace scene;
	using namespace exporter;
	using namespace tracer;

	class Application {

		public:
			static Application& getInstance() {
				static Application instance;
				return instance;
			}
			void init(int argc, char * argv[]);
			void parseCommandLineArgs(int argc, char * argv[]);
			void start();
			void run();
			void stop();
			void addToDataset(Data dat);
			void incrementTracing();
			SceneManager getSceneManager();
			list<Data> dataSet;
			list<Ray> rays;
			Config getApplicationConfig();
			Config getCelestialConfig();
			void setCelestialConfig(Config conf);
			void setApplicationConfig(Config conf);

		private:
			Application() {
				_isRunning = false;
				_numTracings = 0;
			}
			Application(Application const&);      // Don't Implement.
			void operator = (Application const&); // Don't implement
			void createScene();
			void flushScene();
			bool _isRunning;
			int _numTracings;
			Config _celestialConfig;
			Config _applicationConfig;
			const char * _applicationConfigFile = "config/config.json";
			const char * _celestialConfigFile = "";
			const char * _outputFile = "Debug/data.dat";
			int _verbosity = boost::log::trivial::warning;
			int _parallelism = 0;
			int _iterations = 0;
			SceneManager _scm;
			MatlabExporter _me;

	};

} /* namespace core */
} /* namespace raytracer */

#endif /* APPLICATION_H_ */
