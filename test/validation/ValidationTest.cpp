#include "gtest/gtest.h"
#include <list>
#include "../../src/scene/Ionosphere.h"
#include "../../src/tracer/Ray.h"
#include "../../src/core/Application.h"
#include "../../src/core/Config.h"
#include "../../src/exporter/Data.h"
#include "../../src/exporter/MatlabExporter.h"

namespace {

	using namespace raytracer::exporter;
	using namespace raytracer::math;
	using namespace raytracer::scene;
	using namespace raytracer::core;

	class ValidationTest : public ::testing::Test {

		public:
			double R = 3390e3;
	};

	TEST_F(ValidationTest, DISABLED_ElectronDensityProfiles) {

		list<Data> dataSet;

		Config celestialConfig = Config("config/scenario_LS180_SA75_DS0.json");

		double SZA = 71 * Constants::PI / 180;

		int dh = 100;
		const Json::Value ionosphereConfig = celestialConfig.getObject("ionosphere")["layers"];
		for (int h = 50e3; h < 250e3; h += dh) {
			Vector3d N = Vector3d(sin(SZA), cos(SZA), 0).norm();
			Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
			mesh.size = 1000;
			Ionosphere io = Ionosphere(mesh);
			io.layerHeight = dh;

			ASSERT_NEAR(SZA, io.getMesh().normal.angle(Vector3d::SUBSOLAR), 0.001);

			for (int idx = 0; idx < ionosphereConfig.size(); idx++) {

				double electronPeakDensity = atof(ionosphereConfig[idx].get("electronPeakDensity", "").asCString());
				double peakProductionAltitude = ionosphereConfig[idx].get("peakProductionAltitude", "").asDouble();
				double neutralScaleHeight = ionosphereConfig[idx].get("neutralScaleHeight", "").asDouble();

				ASSERT_LT(0, electronPeakDensity);
				ASSERT_LT(0, peakProductionAltitude);
				ASSERT_LT(0, neutralScaleHeight);

				io.superimposeElectronNumberDensity(electronPeakDensity, peakProductionAltitude, neutralScaleHeight);

				ASSERT_NEAR(h, io.getAltitude(), 1);
			}

			Data d;
			d.x = mesh.centerpoint.x;
			d.y = mesh.centerpoint.y;
			d.n_e = io.getElectronNumberDensity();
			dataSet.push_back(d);
		}

		MatlabExporter me;
		me.dump("Debug/data_LS180_SA75_DS0_SZA71.dat", dataSet);
	}

	TEST_F(ValidationTest, DISABLED_ElectronDensityProfilesScanSZA) {

		list<Data> dataSet;

		Config celestialConfig = Config("config/scenario_LS180_SA75_DS0.json");

		int dh = 100;
		const Json::Value ionosphereConfig = celestialConfig.getObject("ionosphere")["layers"];
		for (double SZA = 0; SZA < Constants::PI/2; SZA += Constants::PI/180) {
			for (int h = 50e3; h < 250e3; h += dh) {
				Vector3d N = Vector3d(sin(SZA), cos(SZA), 0).norm();
				Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
				mesh.size = 1000;
				Ionosphere io = Ionosphere(mesh);
				io.layerHeight = dh;

				ASSERT_NEAR(SZA, io.getMesh().normal.angle(Vector3d::SUBSOLAR), 0.001);

				for (int idx = 0; idx < ionosphereConfig.size(); idx++) {

					double electronPeakDensity = atof(ionosphereConfig[idx].get("electronPeakDensity", "").asCString());
					double peakProductionAltitude = ionosphereConfig[idx].get("peakProductionAltitude", "").asDouble();
					double neutralScaleHeight = ionosphereConfig[idx].get("neutralScaleHeight", "").asDouble();

					ASSERT_LT(0, electronPeakDensity);
					ASSERT_LT(0, peakProductionAltitude);
					ASSERT_LT(0, neutralScaleHeight);

					io.superimposeElectronNumberDensity(electronPeakDensity, peakProductionAltitude, neutralScaleHeight);

					ASSERT_NEAR(h, io.getAltitude(), 1);
				}

				Data d;
				d.x = mesh.centerpoint.x;
				d.y = mesh.centerpoint.y;
				d.n_e = io.getElectronNumberDensity();
				d.theta_0 = SZA;
				dataSet.push_back(d);
			}
		}

		MatlabExporter me;
		me.dump("Debug/data_LS180_SA75_DS0_ScanSZA.dat", dataSet);
	}

	TEST_F(ValidationTest, DISABLED_Nielsen2007AbsorptionScanSZA) {

		list<Data> dataSet;

		Config celestialConfig = Config("config/scenario_nielsen2007.json");

		double angularStepSize = Constants::PI/180;

		int dh = 1000;
		int numRay = 0;

		const Json::Value ionosphereConfig = celestialConfig.getObject("ionosphere")["layers"];
		for (double f = 3e6; f <= 4e6; f += 2e5) {
			for (double SZA = 0; SZA < Constants::PI/2; SZA += angularStepSize) {

				Ray r;
				r.rayNumber = ++numRay;
				r.frequency = f;
				r.o = Vector3d(0, 0, 0);
				r.setAngle(Constants::PI/2 - SZA);
				Vector3d N = Vector3d(sin(SZA), cos(SZA), 0).norm();

				for (int h = 90e3; h < 300e3; h += dh) {
					Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
					mesh.size = 1000;
					Ionosphere io = Ionosphere(mesh);
					io.layerHeight = dh;

					ASSERT_NEAR(SZA, io.getMesh().normal.angle(Vector3d::SUBSOLAR), 0.001);

					for (int idx = 0; idx < ionosphereConfig.size(); idx++) {

						double electronPeakDensity = atof(ionosphereConfig[idx].get("electronPeakDensity", "").asCString());
						double peakProductionAltitude = ionosphereConfig[idx].get("peakProductionAltitude", "").asDouble();
						double neutralScaleHeight = ionosphereConfig[idx].get("neutralScaleHeight", "").asDouble();

						ASSERT_LT(0, electronPeakDensity);
						ASSERT_LT(0, peakProductionAltitude);
						ASSERT_LT(0, neutralScaleHeight);

						io.superimposeElectronNumberDensity(electronPeakDensity, peakProductionAltitude, neutralScaleHeight);

						ASSERT_NEAR(h, io.getAltitude(), 1);
					}

					if (io.determineWaveBehaviour(&r) == Ray::wave_refraction) {
						io.attenuate(&r);
					} else {
						r.signalPower = 0;
					}
				}

				Data d;
				d.rayNumber = r.rayNumber;
				d.frequency = r.frequency;
				d.signalPower = r.signalPower;
				d.theta_0 = SZA;
				dataSet.push_back(d);
			}
		}

		MatlabExporter me("Debug/data_nielsen2007absorption.dat");
		me.dump("Debug/data_nielsen2007absorption.dat", dataSet);
	}

	TEST_F(ValidationTest, DISABLED_Withers2011AbsorptionTest) {

		list<Data> dataSet;
		float SZA = 0;
		int numRay = 0;
		int dh = 100;
		MatlabExporter me("Debug/data_withers2011_EP.dat");

		Config celestialConfig = Config("config/scenario_withers2011_M2.json");
		Application::getInstance().setCelestialConfig(celestialConfig);
		const Json::Value ionosphereConfig = celestialConfig.getObject("ionosphere")["layers"];

		double peakProductionAltitude = 35e3;//ionosphereConfig[0].get("peakProductionAltitude", "").asDouble();
		double neutralScaleHeight = ionosphereConfig[0].get("neutralScaleHeight", "").asDouble();

		// assume single-layer ionosphere as that is how withers models the absorption
		if (ionosphereConfig.size() > 1) {
			std::cerr << "ionosphere config has too many layers! Maximum is 1 for this validation test.\n";
		}

		for (int m = 7; m <= 12; m++) {

			double electronPeakDensity = pow(10, m);
			double n = 6.0;

			while (n < 11.0) {

				double f = pow(10, n);
				n += 0.01;

				Ray r;
				r.rayNumber = ++numRay;
				r.frequency = f;
				r.o = Vector3d(0, 0, 0);
				r.setAngle(Constants::PI/2 - SZA);
				Vector3d N = Vector3d(sin(SZA), cos(SZA), 0).norm();

				for (int h = 20e3; h < 250e3; h += dh) {
					Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
					mesh.size = 1000;
					Ionosphere io = Ionosphere(mesh);
					io.layerHeight = dh;

					ASSERT_NEAR(SZA, io.getMesh().normal.angle(Vector3d::SUBSOLAR), 0.001);

					io.superimposeElectronNumberDensity(electronPeakDensity, peakProductionAltitude, neutralScaleHeight);

					if (io.determineWaveBehaviour(&r) == Ray::wave_refraction) {
						io.attenuate(&r);
					} else {
						r.signalPower = 0;
					}
				}

				Data d;
				d.rayNumber = r.rayNumber;
				d.frequency = r.frequency;
				d.signalPower = r.signalPower;
				d.theta_0 = SZA;
				d.n_e = electronPeakDensity;
				dataSet.push_back(d);
			}
		}

		me.dump("Debug/data_withers2011_EP.dat", dataSet);
	}
}
