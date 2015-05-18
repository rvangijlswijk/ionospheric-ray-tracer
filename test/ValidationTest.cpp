#include "gtest/gtest.h"
#include <list>
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/core/Config.h"
#include "../src/exporter/Data.h"
#include "../src/exporter/MatlabExporter.h"

namespace {

	using namespace raytracer::exporter;
	using namespace raytracer::math;
	using namespace raytracer::scene;
	using namespace raytracer::core;

	class ValidationTest : public ::testing::Test {
	};

	TEST_F(ValidationTest, ElectronDensityProfiles) {

		list<Data> dataSet;

		Config celestialConfig = Config("config/scenario_LS180_SA75_DS0.json");

		double SZA = 85 * Constants::PI / 180;
		double R = 3390e3;

		int dh = 100;
		const Json::Value ionosphereConfig = celestialConfig.getArray("ionosphere");
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
		me.dump("Debug/data_LS180_SA75_DS0_SZA85.dat", dataSet);
	}

	TEST_F(ValidationTest, ElectronDensityProfilesScanSZA) {

		list<Data> dataSet;

		Config celestialConfig = Config("config/scenario_LS180_SA75_DS0.json");

		double R = 3390e3;

		int dh = 100;
		const Json::Value ionosphereConfig = celestialConfig.getArray("ionosphere");
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
}
