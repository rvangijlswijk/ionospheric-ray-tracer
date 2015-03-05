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
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/thread.hpp>
#include "Timer.cpp"
#include "Config.h"
#include "../scene/SceneManager.h"
#include "../scene/Ionosphere.h"
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
			void init();
			void start();
			void run();
			void stop();
			void addToDataset(Data dat);
			void incrementTracing();
			SceneManager getSceneManager();
			list<Data> dataSet;
			list<Ray> rays;

		private:
			Application() {
				isRunning = false;
				numTracings = 0;
			}
			Application(Application const&);      // Don't Implement.
			void operator = (Application const&); // Don't implement
			void createScene();
			void flushScene();
			bool isRunning;
			SceneManager scm;
			int numTracings;

	};

} /* namespace core */
} /* namespace raytracer */

#endif /* APPLICATION_H_ */
