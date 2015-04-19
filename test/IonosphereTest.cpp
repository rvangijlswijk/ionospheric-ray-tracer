#include "gtest/gtest.h"
#include <list>
#include "../src/core/Application.h"
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

				conf = Config("config/scenario_test.json");
				Application::getInstance().setCelestialConfig(conf);

				Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 100e3, 0));
				io.setMesh(mesh);
				io.setup();
				io.layerHeight = 1000;
				io.setElectronPeakDensity(2.5e11);
				io.setPeakProductionAltitude(125e3);
				// a layer with an SZA angle of 1 degree
				Plane3d mesh2 = Plane3d(Vector3d(0.0175, 1, 0), Vector3d(49204, 3514.3e3, 0));
				io2.setMesh(mesh2);
				io2.setup();
				io2.layerHeight = 1000;
				io2.setElectronPeakDensity(2.5e11);
				io2.setPeakProductionAltitude(125e3);
				Plane3d mesh3 = Plane3d(Vector3d(1, 0, 0), Vector3d(3390e3 + 100e3, 0, 0));
				io3.setMesh(mesh3);
				io3.setup();
				io3.layerHeight = 1000;
				io3.setElectronPeakDensity(2.5e11);
				io3.setPeakProductionAltitude(125e3);

				r.originalAngle = 30 * Constants::PI / 180.0; // SZA = 30 deg
				r.previousRefractiveIndex = 1.0;
				r.setNormalAngle(r.originalAngle);
				r.frequency = 4e6;

				r2.o = Vector3d(0, 3514.8e3, 0);
				r2.originalAngle = 80 * Constants::PI / 180.0; // SZA = 30 deg
				r2.previousRefractiveIndex = 1.0;
				r2.setAngle(10 * Constants::PI / 180.0);
				r2.frequency = 5e6;
			}

			Ionosphere io, io2, io3;
			Ray r, r2;
			Config conf;
	};

	TEST_F(IonosphereTest, ElectronPeakDensity) {

		ASSERT_EQ(2.5e11, io.getElectronPeakDensity());
	}

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

		ASSERT_NEAR(0.972, io.getRefractiveIndex(&r, Ionosphere::REFRACTION_KELSO), 0.01);
		ASSERT_TRUE(std::isnan(io2.getRefractiveIndex(&r, Ionosphere::REFRACTION_KELSO)));
		ASSERT_NEAR(1, io3.getRefractiveIndex(&r, Ionosphere::REFRACTION_KELSO), 0.01);
		ASSERT_NEAR(0.982, io.getRefractiveIndex(&r2, Ionosphere::REFRACTION_KELSO), 0.01);
		ASSERT_NEAR(0.440, io2.getRefractiveIndex(&r2, Ionosphere::REFRACTION_KELSO), 0.01);
		ASSERT_NEAR(1, io3.getRefractiveIndex(&r2, Ionosphere::REFRACTION_KELSO), 0.01);
	}

	TEST_F(IonosphereTest, IncidentAngle) {

		ASSERT_NEAR(0.524, io.getIncidentAngle(&r), 0.001);
		ASSERT_NEAR(0.506, io2.getIncidentAngle(&r), 0.001);
		ASSERT_NEAR(1.379, io2.getIncidentAngle(&r2), 0.001);
		ASSERT_NEAR(0.175, io3.getIncidentAngle(&r2), 0.001);

		Ionosphere rio = Ionosphere();
		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 100, 0));
		rio.setMesh(mesh);

		Ray ri = Ray();
		ri.d = Vector3d(0.707, -0.707, 0);

		ASSERT_NEAR(0.786, rio.getIncidentAngle(&ri), 1e-3);
	}

	TEST_F(IonosphereTest, DetermineWaveBehaviour) {

		int behaviour = io.determineWaveBehaviour(&r);
		int behaviour2 = io2.determineWaveBehaviour(&r2);

		ASSERT_EQ(Ray::wave_refraction, behaviour);
		ASSERT_EQ(Ray::wave_reflection, behaviour2);
	}

	TEST_F(IonosphereTest, CollisionFrequency) {

		Ionosphere cio, cio2, cio3, cio4;
		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 100e3, 0));
		cio.setMesh(mesh);
		cio.setup();
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 150e3, 0));
		cio2.setMesh(mesh);
		cio2.setup();
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 200e3, 0));
		cio3.setMesh(mesh);
		cio3.setup();
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 80e3, 0));
		cio4.setMesh(mesh);
		cio4.setup();

		ASSERT_NEAR(3.424e6, cio.getCollisionFrequency(), 1e4);
		ASSERT_NEAR(3.787e4, cio2.getCollisionFrequency(), 1e2);
		ASSERT_NEAR(418.8, cio3.getCollisionFrequency(), 4);
		ASSERT_NEAR(2.075e7, cio4.getCollisionFrequency(), 1e5);
	}

	TEST_F(IonosphereTest, Refract) {

		Ionosphere rio = Ionosphere();
		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 100, 0));
		rio.setMesh(mesh);
		rio.setup();
		rio.setElectronPeakDensity(1);
		rio.setPeakProductionAltitude(125e3);

		Ray* rref = new Ray;
		rref->frequency = 5e6;

		rref->o = Vector3d(0, 0, 0);
		rref->d = Vector3d(0.5, 0.867, 0);
		rref->previousRefractiveIndex = 1/1.068;
		rio.refract(rref);

		ASSERT_NEAR(0.534, rref->d.x, 1e-3);
		ASSERT_NEAR(1.005, rref->d.y, 1e-3);
		ASSERT_NEAR(0, rref->d.z, 1e-3);

		rref->o = Vector3d(0, 0, 0);
		rref->d = Vector3d(0.5 * sqrt(2), -0.5 * sqrt(2), 0);
		rref->previousRefractiveIndex = 1/0.9;
		rio.refract(rref);

		ASSERT_NEAR(0.636, rref->d.x, 1e-3);
		ASSERT_NEAR(-0.771, rref->d.y, 1e-3);
		ASSERT_NEAR(0, rref->d.z, 1e-3);
	}

	/**
	 * if a ray transverses from a medium n1 to a medium n2 where n1 > n2,
	 * then the incident angle theta_i is lower than the reflected angle theta_r
	 * w.r.t. the normal: theta_i < theta_r
	 * else if n1 < n2, then theta_i > theta_r
	 */
	TEST_F(IonosphereTest, RefractSnelliusLaw) {

		Ionosphere rio = Ionosphere();
		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 100, 0));
		rio.setMesh(mesh);
		rio.setup();
		rio.setElectronPeakDensity(1);
		rio.setPeakProductionAltitude(125e3);

		Ray* rref = new Ray;
		rref->frequency = 5e6;

		rref->o = Vector3d(0, 0, 0);
		rref->d = Vector3d(0.5, 0.867, 0); // theta_i = 30 deg, theta_0 = 60 deg
		rref->previousRefractiveIndex = 1.1; // n1 > n2
		double factor_i = rref->d.y / rref->d.x;
		double theta_i = rio.getIncidentAngle(rref);
		rio.refract(rref);
		double factor_r = rref->d.y / rref->d.x;
		double theta_r = acos(rref->d * rio.mesh3d.normal / (rref->d.magnitude() * rio.mesh3d.normal.magnitude()));

		ASSERT_NEAR(0.524, theta_i, 1e-3);	// theta_i = 30 deg
		ASSERT_GT(factor_i, factor_r);
		ASSERT_LT(theta_i, theta_r);

		rref->previousRefractiveIndex = 0.9; // n2 > n1
		factor_i = rref->d.y / rref->d.x;
		theta_i = rio.getIncidentAngle(rref);
		rio.refract(rref);
		factor_r = rref->d.y / rref->d.x;
		theta_r = acos(rref->d * rio.mesh3d.normal / (rref->d.magnitude() * rio.mesh3d.normal.magnitude()));

		ASSERT_LT(factor_i, factor_r);
		ASSERT_GT(theta_i, theta_r);
	}

	TEST_F(IonosphereTest, Reflect) {

		Ionosphere rio = Ionosphere();
		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 100, 0));
		rio.setMesh(mesh);
		rio.setup();
		rio.setElectronPeakDensity(1);
		rio.setPeakProductionAltitude(125e3);

		Ray* rref = new Ray;
		rref->frequency = 5e6;

		rref->o = Vector3d(0, 0, 0);
		rref->d = Vector3d(0.5 * sqrt(2), -0.5 * sqrt(2), 0);
		rref->previousRefractiveIndex = 1/0.9;
		rio.reflect(rref);

		ASSERT_NEAR(0.707, rref->d.x, 1e-3);
		ASSERT_NEAR(0.707, rref->d.y, 1e-3);
		ASSERT_NEAR(0, rref->d.z, 1e-3);
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
			Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + h, 0));
			ion.setMesh(mesh);
			ion.layerHeight = 1000;
			ion.setup();
			ion.setElectronPeakDensity(2.5e11);
			ion.setPeakProductionAltitude(125e3);
			ion.attenuate(&rA, ion.layerHeight);

			ASSERT_NEAR(h, ion.getAltitude(), 1);
		}

		ASSERT_NEAR(-1.5, rA.signalPower, 0.5);

		// M1 layer
		rA.previousRefractiveIndex = 1.0;
		rA.signalPower = 0;
		for (int h = 80e3; h <= 200e3; h += 1000) {
			Ionosphere ion;
			Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + h, 0));
			ion.setMesh(mesh);
			ion.layerHeight = 1000;
			ion.setup();
			ion.setElectronPeakDensity(1e11);
			ion.setPeakProductionAltitude(100e3);
			ion.attenuate(&rA, ion.layerHeight);

			ASSERT_NEAR(h, ion.getAltitude(), 1);
		}

		ASSERT_NEAR(-9, rA.signalPower, 0.5);
	}

	TEST_F(IonosphereTest, TEC) {

		ASSERT_NEAR(1.095e13, io.getTEC(), 1.1e10);
		ASSERT_NEAR(2.5e14, io2.getTEC(), 1.1e11);
		ASSERT_EQ(0, io3.getTEC());
	}

	TEST_F(IonosphereTest, RangeDelay) {

		r.rangeDelay = 0;
		io.rangeDelay(&r);
		ASSERT_NEAR(0.276, r.rangeDelay, 0.001);

		r.rangeDelay = 0;
		io2.rangeDelay(&r);
		ASSERT_NEAR(6.295, r.rangeDelay, 0.001);

		r.rangeDelay = 0;
		io3.rangeDelay(&r);
		ASSERT_NEAR(0, r.rangeDelay, 0.001);
	}

	TEST_F(IonosphereTest, TimeDelay) {

		r.timeDelay = 0;
		io.timeDelay(&r);
		ASSERT_NEAR(9.168e-8, r.timeDelay, 0.001);

		r.timeDelay = 0;
		io2.timeDelay(&r);
		ASSERT_NEAR(2.093e-6, r.timeDelay, 0.001);

		r.timeDelay = 0;
		io3.timeDelay(&r);
		ASSERT_NEAR(0, r.timeDelay, 0.001);
	}

	TEST_F(IonosphereTest, PhaseAdvance) {

		r.phaseAdvance = 0;
		io.phaseAdvance(&r);
		ASSERT_NEAR(2.310, r.phaseAdvance, 0.001);

		r.phaseAdvance = 0;
		io2.phaseAdvance(&r);
		ASSERT_NEAR(52.734, r.phaseAdvance, 0.001);

		r.phaseAdvance = 0;
		io3.phaseAdvance(&r);
		ASSERT_NEAR(0, r.phaseAdvance, 0.001);
	}

	TEST_F(IonosphereTest, ExportDataTest) {

		list<Data> dataSet;

		for (int h = 50000; h<500000; h+=100) {
			Data d;
			d.y = h;
			Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + h, 0));
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
