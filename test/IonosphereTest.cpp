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
				Line2f mesh = Line2f(Vector2f(0, 100000), Vector2f(100000,100000));
				io.setMesh(mesh);
				Line2f mesh2 = Line2f(Vector2f(0, 125000), Vector2f(100000,125000));
				io2.setMesh(mesh2);

				r.setSolarZenithAngle(0.524);
				r.frequency = 4e6;
			}

			Ionosphere io, io2;
			Ray r;
	};

	TEST_F(IonosphereTest, ElectronNumberDensity) {

		ASSERT_NEAR(1.21e9, io.getElectronNumberDensity(r), 1.21e7);	// 1% error acceptable
		ASSERT_NEAR(2e11, io2.getElectronNumberDensity(r), 2e5);		// 1 ppm error acceptable (chapman max)
	}

	TEST_F(IonosphereTest, PlasmaFrequency) {

		ASSERT_NEAR(1.96e6, io.getPlasmaFrequency(r), 2e4);
		ASSERT_NEAR(2.52e7, io2.getPlasmaFrequency(r), 2.5e5);
	}

	TEST_F(IonosphereTest, ExportDataTest) {

		list<Data> dataSet;

		for (int h = 50000; h<500000; h+=100) {
			Data d;
			d.y = h;
			Line2f mesh = Line2f(Vector2f(0, h), Vector2f(100000, h));
			io.setMesh(mesh);
			d.n_e = io.getElectronNumberDensity(r);
			d.omega_p = io.getPlasmaFrequency(r);
			dataSet.push_back(d);
		}
		MatlabExporter me;
		me.dump("Debug/data_IonosphereTest.dat", dataSet);
	}

	TEST_F(IonosphereTest, RefractiveIndexSimple) {

		ASSERT_NEAR(0.94, io.getRefractiveIndex(r, Ionosphere::SIMPLE), 0.01);
		ASSERT_NEAR(1.0, io2.getRefractiveIndex(r, Ionosphere::SIMPLE), 0.01);
	}

	TEST_F(IonosphereTest, RefractiveIndexKelso) {

		ASSERT_NEAR(0.98, io.getRefractiveIndex(r, Ionosphere::KELSO), 0.01);
	}
}
