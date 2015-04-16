#include "gtest/gtest.h"
#include "../src/scene/SceneManager.h"
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/math/Vector3d.h"
#include "../src/math/Line3d.h"

namespace {

	using namespace raytracer::scene;
	using namespace raytracer::math;

	class SceneManagerTest : public ::testing::Test {

		protected:
			void SetUp() {

				sm = SceneManager();
				io = Ionosphere();
				Plane3d mesh = Plane3d(Vector3d(0, 1, 0), Vector3d(100e3, 3390e3 + 100e3, 0));
				mesh.size = 1e3;
				io.setMesh(mesh);

				sm.addToScene(&io);
			}

			SceneManager sm;
			Ionosphere io;
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

		Intersection is = sm.intersect(&r, rayLine);
		Plane3d mesh = (*is.g).mesh3d;

		ASSERT_NE(GeometryType::none, is.o);
		ASSERT_NEAR((mesh.centerpoint.y / rayLine.destination.y) * mesh.centerpoint.x, is.pos.x, 10);
		ASSERT_EQ(3390e3 + 100e3, is.pos.y);
		ASSERT_NEAR(100e3, mesh.centerpoint.x, 10);
		ASSERT_NEAR(3390e3 + 100e3, mesh.centerpoint.y, 10);
		ASSERT_NEAR(0, mesh.centerpoint.z, 10);
	}
}
