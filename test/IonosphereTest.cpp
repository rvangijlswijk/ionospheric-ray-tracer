#include "gtest/gtest.h"
#include <list>
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/exporter/Data.h"
#include "../src/exporter/MatlabExporter.h"

namespace {

	using namespace raytracer::scene;
	using namespace raytracer::tracer;
	using namespace raytracer::exporter;

	class IonosphereTest : public ::testing::Test {

		protected:
			void SetUp() {
				Line2d mesh = Line2d(Vector2d(-100e3, 3390e3 + 100e3), Vector2d(100e3, 3390e3 + 100e3));
				io.setMesh(mesh);
				Line2d mesh2 = Line2d(Vector2d(0, 3515e3), Vector2d(98408.25, 3513.6e3));
				io2.setMesh(mesh2);
				Line2d mesh3 = Line2d(Vector2d(3390e3 + 100e3, 100e3), Vector2d(3390e3 + 100e3, -100e3));
				io3.setMesh(mesh3);

				r.setNormalAngle(0.524);
				r.frequency = 4e6;
			}

			Ionosphere io, io2, io3;
			Ray r;
	};

	TEST_F(IonosphereTest, SolarZenithAngle) {

		ASSERT_NEAR(0, io.getSolarZenithAngle2f(), 0.001);
		ASSERT_NEAR(0.014, io2.getSolarZenithAngle2f(), 0.001);
		ASSERT_NEAR(1.571, io3.getSolarZenithAngle2f(), 0.001);
	}

	TEST_F(IonosphereTest, GetAltitudeTest) {

		ASSERT_NEAR(100e3, io.getAltitude(), 100);
		ASSERT_NEAR(125e3, io2.getAltitude(), 1000);
		ASSERT_NEAR(100e3, io3.getAltitude(), 100);
	}

	TEST_F(IonosphereTest, ElectronNumberDensity) {

		ASSERT_NEAR(1.1e10, io.getElectronNumberDensity(), 1.1e8);		// 1% error acceptable
		ASSERT_NEAR(2.5e11, io2.getElectronNumberDensity(), 2.5e8);		// 0.1% error acceptable (chapman max)
		ASSERT_NEAR(0, io3.getElectronNumberDensity(), 2.5e8);			// SZA = 90 deg
	}

	TEST_F(IonosphereTest, PlasmaFrequency) {

		ASSERT_NEAR(28.2e6, io.getPlasmaFrequency(), 1e4);
		ASSERT_NEAR(28.2e6, io2.getPlasmaFrequency(), 1e4);
		ASSERT_NEAR(28.2e6, io3.getPlasmaFrequency(), 1e4);
	}

	TEST_F(IonosphereTest, RefractiveIndexSimple) {

		ASSERT_NEAR(0.94, io.getRefractiveIndex(r, Ionosphere::SIMPLE), 0.01);
		ASSERT_NEAR(1.0, io2.getRefractiveIndex(r, Ionosphere::SIMPLE), 0.01);
	}

	TEST_F(IonosphereTest, RefractiveIndexKelso) {

		ASSERT_NEAR(0.98, io.getRefractiveIndex(r, Ionosphere::KELSO), 0.01);
	}

	TEST_F(IonosphereTest, ExportDataTest) {

		list<Data> dataSet;

		for (int h = 50000; h<500000; h+=100) {
			Data d;
			d.y = h;
			Line2d mesh = Line2d(Vector2d(0, h), Vector2d(100000, h));
			io.setMesh(mesh);
			d.n_e = io.getElectronNumberDensity();
			d.omega_p = io.getPlasmaFrequency();
			dataSet.push_back(d);
		}
		MatlabExporter me;
		me.dump("Debug/data_IonosphereTest.dat", dataSet);
	}
}
