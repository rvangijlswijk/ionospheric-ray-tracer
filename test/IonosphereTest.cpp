#include "gtest/gtest.h"
#include <list>
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/exporter/Data.h"
#include "../src/exporter/MatlabExporter.h"
#include "../src/math/Constants.h"
#include "../src/core/Config.h"

namespace {

	using namespace raytracer::scene;
	using namespace raytracer::tracer;
	using namespace raytracer::exporter;
	using namespace raytracer::math;
	using namespace raytracer::core;

	class IonosphereTest : public ::testing::Test {

		protected:
			void SetUp() {

				Config::getInstance().loadFromFile("config/mars.json");

				Line2d mesh = Line2d(Vector2d(-100e3, 3390e3 + 100e3), Vector2d(100e3, 3390e3 + 100e3));
				io.setMesh(mesh);
				io.setup();
				Line2d mesh2 = Line2d(Vector2d(0, 3515e3), Vector2d(98408.25, 3513.6e3));
				io2.setMesh(mesh2);
				io2.setup();
				Line2d mesh3 = Line2d(Vector2d(3390e3 + 100e3, 100e3), Vector2d(3390e3 + 100e3, -100e3));
				io3.setMesh(mesh3);
				io3.setup();

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

			Ionosphere io, io2, io3;
			Ray r, r2;
	};

	TEST_F(IonosphereTest, SolarZenithAngle) {

		ASSERT_NEAR(0, io.getSolarZenithAngle2d(), 0.001);
		ASSERT_NEAR(0.014, io2.getSolarZenithAngle2d(), 0.001);
		ASSERT_NEAR(1.571, io3.getSolarZenithAngle2d(), 0.001);
	}

	TEST_F(IonosphereTest, Altitude) {

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

		ASSERT_NEAR(5.9e6, io.getPlasmaFrequency(), 1e4);
		ASSERT_NEAR(28.2e6, io2.getPlasmaFrequency(), 1e4);
		ASSERT_NEAR(0, io3.getPlasmaFrequency(), 1e4);
	}

	TEST_F(IonosphereTest, RefractiveIndexSimple) {

		ASSERT_NEAR(0.94, io.getRefractiveIndex(&r, Ionosphere::REFRACTION_SIMPLE), 0.01);
		ASSERT_NEAR(1.0, io2.getRefractiveIndex(&r, Ionosphere::REFRACTION_SIMPLE), 0.01);
	}

	TEST_F(IonosphereTest, RefractiveIndexKelso) {

		ASSERT_NEAR(0.98, io.getRefractiveIndex(&r, Ionosphere::REFRACTION_KELSO), 0.01);
		ASSERT_NEAR(0.44, io2.getRefractiveIndex(&r2, Ionosphere::REFRACTION_KELSO), 0.01);
	}

	TEST_F(IonosphereTest, IncidentAngle) {

		ASSERT_NEAR(0.524, io.getIncidentAngle(&r), 0.001);
		ASSERT_NEAR(0.510, io2.getIncidentAngle(&r), 0.001);
		ASSERT_NEAR(1.382, io2.getIncidentAngle(&r2), 0.001);
	}

	TEST_F(IonosphereTest, DetermineWaveBehaviour) {

		int behaviour = io.determineWaveBehaviour(&r);
		int behaviour2 = io2.determineWaveBehaviour(&r2);

		ASSERT_EQ(Ray::wave_refraction, behaviour);
		ASSERT_EQ(Ray::wave_reflection, behaviour2);
	}

	TEST_F(IonosphereTest, CollisionFrequency) {

	}

	/**
	 * Test the total attenuation of a radio wave through the ionosphere.
	 * According to Nielsen, 2007 (fig 5) a 4MHz signal at a SZA of 30 deg
	 * should have a total attenuation of ~27 dB through the entire ionosphere.
	 * Losses according to Withers, 2011, section 5.2:
	 * Loss due to M2: ~1.5 dB
	 * Loss due to M1: ~9.0 dB
	 * Loss due to Meteoric layer: ~12.8dB
	 * Loss due to EP: ~0.9 dB
	 * Assuming: SZA = 0 deg
	 */
	TEST_F(IonosphereTest, Attenuate) {

		Ray rA;
		rA.frequency = 5e6;
		rA.previousRefractiveIndex = 1.0;
		rA.setAngle(90 * Constants::PI / 180); // SZA = 0 deg

		// M2 layer
		rA.signalPower = 0;
		for (int h = 80e3; h <= 200e3; h += 1000) {
			Ionosphere ion;
			Line2d mesh = Line2d(Vector2d(-100e3, 3390e3 + h), Vector2d(100e3, 3390e3 + h));
			ion.setMesh(mesh);
			ion.layerHeight = 1000;
			ion.setup();
			ion.attenuate(&rA, ion.layerHeight);

			ASSERT_NEAR(h, ion.getAltitude(), 1);
		}

		ASSERT_NEAR(-1.5, rA.signalPower, 0.1);

		// M1 layer
//		r.signalPower = 0;
//		for (int h = 80e3; h <= 200e3; h += 1000) {
//			Ionosphere ion;
//			ion.peakProductionAltitude = 100e3;
//			ion.maximumProductionRate = 1e11;
//			Line2d mesh = Line2d(Vector2d(-100e3, 3390e3 + h), Vector2d(100e3, 3390e3 + h));
//			ion.setMesh(mesh);
//			ion.layerHeight = 1000;
//			ion.attenuate(&rA);
//
//			ASSERT_NEAR(h, ion.getAltitude(), 1);
//		}
//
//		ASSERT_NEAR(-9, r.signalPower, 0.1);
	}

	TEST_F(IonosphereTest, ExportDataTest) {

		list<Data> dataSet;

		for (int h = 50000; h<500000; h+=100) {
			Data d;
			d.y = h;
			Line2d mesh = Line2d(Vector2d(0, 3390e3 + h), Vector2d(100000, 3390e3 + h));
			io.setMesh(mesh);
			d.n_e = io.getElectronNumberDensity();
			d.omega_p = io.getPlasmaFrequency();
			d.mu_r_sqrt = io.getRefractiveIndex(&r, Ionosphere::REFRACTION_KELSO);
			dataSet.push_back(d);
		}
		MatlabExporter me;
		me.dump("Debug/data_IonosphereTest.dat", dataSet);
	}
}
