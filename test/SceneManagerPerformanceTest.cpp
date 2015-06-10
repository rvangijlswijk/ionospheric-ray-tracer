#include "gtest/gtest.h"
#include <fstream>
#include "../src/core/Timer.cpp"
#include "../src/scene/SceneManager.h"
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/math/Vector3d.h"
#include "../src/math/Line3d.h"
#include "../src/core/Config.h"
#include "../src/core/Application.h"

namespace {

	using namespace raytracer::core;
	using namespace raytracer::scene;
	using namespace raytracer::math;

	class SceneManagerPerformanceTest : public ::testing::Test {

		protected:
			void SetUp() {
				conf = Config("config/scenario_test.json");
				Application::getInstance().setCelestialConfig(conf);
				appConf = Config("config/config.json");
				Application::getInstance().setApplicationConfig(conf);

				R = 3390e3;
			}

			double R;
			Config conf, appConf;
	};

	TEST_F(SceneManagerPerformanceTest, OneMillionMeshes) {

		SceneManager scm = SceneManager();

		// init
		int numSceneObjectsCreated = 0;
		int start = 1000;
		int dh = 20;
		int end = 101e3;
		double angularStepSize = Constants::PI/200.0;
		Ray r = Ray();
		r.frequency = 4e6;
		r.o = Vector3d(0, R, 0);
		Line3d rayLine = Line3d(r.o, Vector3d(R + 100e3, R + 100e3, 0));
		ofstream data;
		data.open("testresults1M.txt");

		// create scene
		for (double theta = -Constants::PI/2 + angularStepSize; theta < Constants::PI/2; theta += angularStepSize) {
			for (int h = start; h < end; h += dh) {

				Vector3d N = Vector3d(cos(theta), sin(theta), 1).norm();
				Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
				mesh.size = angularStepSize * R;
				Ionosphere* io = new Ionosphere(mesh);
				io->layerHeight = dh;

				numSceneObjectsCreated++;
				scm.addToScene(io);
			}
		}

		scm.sortScene();

		// repeat test multiple times
		double tAvg = 0;
		int testRuns = 25;
		double runningTimes[testRuns];
		double variance = 0;
		for (int i = 0; i < testRuns; i++) {

			// perform intersection test
			Timer tmr;

			scm.intersect(r, rayLine);

			double t = tmr.elapsed() * 1000.;
			runningTimes[i] = t;
			tAvg += t/((double)testRuns);
			tmr.reset();
		}

		for (int j = 0; j < testRuns; j++) {

			variance += abs(runningTimes[j] - tAvg) / ((double)testRuns);
		}

		data << "Average elapsed time: " << ::testing::PrintToString(tAvg) << "ms" << endl;
		data << "Standard deviation: su = " << ::testing::PrintToString(sqrt(variance)) << "ms" << endl;
		data << "Objects created: " << ::testing::PrintToString(numSceneObjectsCreated) << "" << endl	;
		data << "Intersections per ms: " << ::testing::PrintToString(numSceneObjectsCreated/tAvg) << "" << endl;
	}
}
