#include "gtest/gtest.h"
#include "../src/core/Application.h"
#include "../src/scene/Atmosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/math/Constants.h"
#include "../src/core/Config.h"

namespace {

	using namespace raytracer::scene;
	using namespace raytracer::tracer;
	using namespace raytracer::math;
	using namespace raytracer::core;

	class AtmosphereTest : public ::testing::Test {

		protected:
			void SetUp() {

				conf = Config();
				conf.loadFromFile("config/scenario_default.json");
				Application::getInstance().setCelestialConfig(conf);

				Line2d mesh = Line2d(Vector2d(-100e3, 3390e3), Vector2d(100e3, 3390e3));
				a1.setMesh(mesh);
				a1.setup();
				mesh = Line2d(Vector2d(-100e3, 3390e3 + 10e3), Vector2d(100e3, 3390e3 + 10e3));
				a2.setMesh(mesh);
				a2.setup();
				mesh = Line2d(Vector2d(-100e3, 3390e3 + 20e3), Vector2d(100e3, 3390e3 + 20e3));
				a3.setMesh(mesh);
				a3.setup();
				mesh = Line2d(Vector2d(-100e3, 3390e3 + 30e3), Vector2d(100e3, 3390e3 + 30e3));
				a4.setMesh(mesh);
				a4.setup();

				r.originalAngle = 30 * Constants::PI / 180.0; // SZA = 30 deg
				r.previousRefractiveIndex = 1.0;
				r.setNormalAngle(r.originalAngle);
				r.frequency = 4e6;

				r2.o = Vector2d(0, 3514.8e3);
				r2.originalAngle = 80 * Constants::PI / 180.0; // SZA = 30 deg
				r2.previousRefractiveIndex = 1.0;
				r2.setAngle(10 * Constants::PI / 180.0);
				r2.frequency = 5e6;
			}

			Atmosphere a1, a2, a3, a4;
			Ray r, r2;
			Config conf;
	};

	TEST_F(AtmosphereTest, RefractiveIndex) {

		ASSERT_NEAR(3.9e-6 + 1, a1.getRefractiveIndex(), 0.1e-6);
		ASSERT_NEAR(1.6e-6 + 1, a2.getRefractiveIndex(), 0.1e-6);
		ASSERT_NEAR(0.6e-6 + 1, a3.getRefractiveIndex(), 0.1e-6);
		ASSERT_NEAR(0.28e-6 + 1, a4.getRefractiveIndex(), 0.1e-6);
	}
}
