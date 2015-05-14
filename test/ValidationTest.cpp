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

		Config celestialConfig = Config("config/scenario_default.json");

		double SZA = 76 * Constants::PI / 180;
		double R = 3390e3;

		int dh = 400;
		const Json::Value ionosphereConfig = celestialConfig.getArray("ionosphere");
		for (int idx = 0; idx < ionosphereConfig.size(); idx++) {

			int hS = ionosphereConfig[idx].get("start", 0).asInt();
			int hE = ionosphereConfig[idx].get("end", 0).asInt();
			double electronPeakDensity = atof(ionosphereConfig[idx].get("electronPeakDensity", "").asCString());
			double peakProductionAltitude = ionosphereConfig[idx].get("peakProductionAltitude", "").asDouble();
			Json::Value stratificationRaw = ionosphereConfig[idx].get("stratification", "");
			const char * stratificationType = stratificationRaw.asCString();
			for (int h = hS; h < hE; h += dh) {
				Vector3d N = Vector3d(sin(SZA), cos(SZA), 0).norm();
				Plane3d mesh = Plane3d(N, Vector3d((R+h)*N.x, (R+h)*N.y, (R+h)*N.z));
				mesh.size = 1000;
				Ionosphere io = Ionosphere(mesh);
				io.layerHeight = dh;
				io.setElectronPeakDensity(electronPeakDensity);
				io.setPeakProductionAltitude(peakProductionAltitude);

				ASSERT_LT(0, io.getElectronPeakDensity());
				ASSERT_LT(0, io.getPeakProductionAltitude());
				ASSERT_LT(50e3, io.getAltitude());
				ASSERT_LT(0, io.getElectronNumberDensity());

				Data d;
				d.x = mesh.centerpoint.x;
				d.y = mesh.centerpoint.y;
				d.n_e = io.getElectronNumberDensity();
				dataSet.push_back(d);
			}
		}

		MatlabExporter me;
		me.dump("Debug/data_LS270_SA75_DS0.dat", dataSet);
	}
}
