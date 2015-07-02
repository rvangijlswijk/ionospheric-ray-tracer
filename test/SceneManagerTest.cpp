#include "gtest/gtest.h"
#include "../src/scene/SceneManager.h"
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/math/Vector3d.h"
#include "../src/math/Line3d.h"
#include "../src/core/Config.h"
#include "../src/core/Application.h"

namespace {

	using namespace raytracer::scene;
	using namespace raytracer::math;

	class SceneManagerTest : public ::testing::Test {

		protected:
			void SetUp() {

				conf = Config("config/scenario_test.json");
				Application::getInstance().setCelestialConfig(conf);
				appConf = Config("config/config.json");
				Application::getInstance().setApplicationConfig(conf);

				sm = SceneManager();
				io = Ionosphere();
				Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(100e3, 3390e3 + 100e3, 0));
				mesh.size = 1e3;
				io.setMesh(mesh);

				sm.addToScene(&io);
			}

			SceneManager sm;
			Ionosphere io;
			Config conf, appConf;
	};

	TEST_F(SceneManagerTest, MeshIsSet) {

		for (Geometry* g : sm.getScene()) {
			ASSERT_EQ(100e3, g->getMesh().centerpoint.x);
			ASSERT_EQ(3390e3 + 100e3, g->getMesh().centerpoint.y);
			ASSERT_EQ(0, g->getMesh().centerpoint.z);
			ASSERT_EQ(0, g->getMesh().normal.x);
			ASSERT_EQ(1, g->getMesh().normal.y);
			ASSERT_EQ(GeometryType::ionosphere, g->type);
		}
	}

	TEST_F(SceneManagerTest, Intersection) {

		raytracer::tracer::Ray r = raytracer::tracer::Ray();
		r.o = Vector3d(0, 0, 0);

		Line3d rayLine = Line3d(Vector3d(0, 0, 0), Vector3d(100e3, 3390e3 + 101e3, 0));

		Intersection is = sm.intersect(r, rayLine);
		Plane3d mesh = is.g->mesh3d;

		ASSERT_NE(GeometryType::none, is.o);
		ASSERT_NEAR((mesh.centerpoint.y / rayLine.destination.y) * mesh.centerpoint.x, is.pos.x, 10);
		ASSERT_EQ(3390e3 + 100e3, is.pos.y);
		ASSERT_NEAR(100e3, mesh.centerpoint.x, 10);
		ASSERT_NEAR(3390e3 + 100e3, mesh.centerpoint.y, 10);
		ASSERT_NEAR(0, mesh.centerpoint.z, 10);
	}

	TEST_F(SceneManagerTest, SortScene) {

		SceneManager scm2 = SceneManager();

		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 10, 0));
		Geometry g1 = Geometry(mesh);
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 13, 0));
		Geometry g2 = Geometry(mesh);
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 14, 0));
		Geometry g3 = Geometry(mesh);
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 12, 0));
		Geometry g4 = Geometry(mesh);
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 3390e3 + 11, 0));
		Geometry g5 = Geometry(mesh);

		scm2.addToScene(&g1);
		scm2.addToScene(&g2);
		scm2.addToScene(&g3);
		scm2.addToScene(&g4);
		scm2.addToScene(&g5);

		scm2.sortScene();

		double lastAlt = 0;
		for (Geometry* g : scm2.getScene()) {

			ASSERT_GT(g->altitude, 0);
			ASSERT_GT(g->altitude, lastAlt);
			lastAlt = g->altitude;
			ASSERT_GT(lastAlt, 0);
		}
	}

	TEST_F(SceneManagerTest, IntersectionOutOfBounds) {

		raytracer::tracer::Ray r = raytracer::tracer::Ray();
		r.o = Vector3d(0, 0, 0);

		Line3d rayLine = Line3d(Vector3d(0, 0, 0), Vector3d(100e3, 3390e3 + 99e3, 0));

		Intersection is = sm.intersect(r, rayLine);

		ASSERT_EQ(GeometryType::none, is.o);
	}

	TEST_F(SceneManagerTest, IntersectionAngled) {

		raytracer::tracer::Ray r = raytracer::tracer::Ray();
		r.o = Vector3d(0, 0, 0);

		double angle = Constants::PI/3;
		double offset  = 250;

		Line3d rayLine = Line3d();
		rayLine.origin = Vector3d(100e3 - 100e3/tan(angle) - offset, 3390e3, 0);
		rayLine.destination.x = 100e3 / tan(angle) + 100e3 - offset;
		rayLine.destination.y = 200e3 + 3390e3;

		Intersection is = sm.intersect(r, rayLine);
		Plane3d mesh = is.g->mesh3d;

		ASSERT_NEAR((rayLine.destination.y - rayLine.origin.y) / (rayLine.destination.x - rayLine.origin.x), tan(angle), 1e-3);
		ASSERT_EQ(GeometryType::ionosphere, is.o);
		ASSERT_NEAR(100e3 - offset, is.pos.x, 10);
		ASSERT_EQ(3390e3 + 100e3, is.pos.y);
		ASSERT_NEAR(100e3, mesh.centerpoint.x, 10);
		ASSERT_NEAR(3390e3 + 100e3, mesh.centerpoint.y, 10);
		ASSERT_NEAR(0, mesh.centerpoint.z, 10);

		rayLine.origin.x -= 1e3;
		rayLine.destination.x -= 1e3;

		is = sm.intersect(r, rayLine);
		ASSERT_EQ(GeometryType::none, is.o);
	}

	TEST_F(SceneManagerTest, IntersectionMultiple) {

		Ionosphere io4 = Ionosphere();
		Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(100e3, 3390e3 + 101e3, 0));
		mesh.size = 1e3;
		io4.setMesh(mesh);

		sm.addToScene(&io4);

		Ionosphere io2 = Ionosphere();
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(100e3, 3390e3 + 102e3, 0));
		mesh.size = 1e3;
		io2.setMesh(mesh);

		sm.addToScene(&io2);

		Ionosphere io3 = Ionosphere();
		mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(100e3, 3390e3 + 103e3, 0));
		mesh.size = 1e3;
		io3.setMesh(mesh);

		sm.addToScene(&io3);

		raytracer::tracer::Ray r = raytracer::tracer::Ray();
		r.o = Vector3d(0, 0, 0);

		Line3d rayLine = Line3d(Vector3d(0, 0, 0), Vector3d(100e3, 3390e3 + 105e3, 0));

		Intersection is = sm.intersect(r, rayLine);
		Plane3d mesh2 = is.g->mesh3d;

		ASSERT_NE(GeometryType::none, is.o);
		ASSERT_NEAR((mesh2.centerpoint.y / rayLine.destination.y) * mesh2.centerpoint.x, is.pos.x, 10);
		ASSERT_EQ(3390e3 + 100e3, is.pos.y);
		ASSERT_NEAR(100e3, mesh2.centerpoint.x, 10);
		ASSERT_NEAR(3390e3 + 100e3, mesh2.centerpoint.y, 10);
		ASSERT_NEAR(0, mesh2.centerpoint.z, 10);

	}
}
