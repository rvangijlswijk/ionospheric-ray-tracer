#include "gtest/gtest.h"
#include "../src/scene/SceneManager.h"
#include "../src/scene/Ionosphere.h"
#include "../src/tracer/Ray.h"
#include "../src/math/Vector2d.h"
#include "../src/math/Line2d.h"

namespace {

	using namespace raytracer::scene;
	using namespace raytracer::math;

	class SceneManagerTest : public ::testing::Test {

		protected:
			void SetUp() {

				sm = SceneManager();
				io = Ionosphere(Vector2d(-100e3, 3390e3 + 100e3), Vector2d(100e3, 3390e3 + 100e3));

				sm.addToScene(&io);
			}

			SceneManager sm;
			Ionosphere io;
	};

	TEST_F(SceneManagerTest, MeshIsSet) {

		for (Geometry* g : sm.getScene()) {
			ASSERT_EQ(-100e3, g->getMesh().begin.x);
			ASSERT_EQ(3390e3 + 100e3, g->getMesh().begin.y);
			ASSERT_EQ(100e3, g->getMesh().end.x);
			ASSERT_EQ(3390e3 + 100e3, g->getMesh().end.y);
		}
	}

	TEST_F(SceneManagerTest, Intersection) {

		raytracer::tracer::Ray r = raytracer::tracer::Ray();
		r.o = Vector2d(0, 0);

		Line2d rayLine = Line2d(Vector2d(0, 0), Vector2d(0, 3390e3 + 101e3));

		Intersection is = sm.intersect(r, rayLine);
		Line2d mesh = (*is.g).mesh2d;
		cout << "hit.g: " << mesh.end.x << " " << mesh.end.y;

		ASSERT_EQ(0, is.pos.x);
		ASSERT_EQ(3390e3 + 100e3, is.pos.y);
		ASSERT_NEAR(-100e3, mesh.begin.x, 10);
		ASSERT_EQ(3390e3 + 100e3, mesh.begin.y);
		ASSERT_EQ(100e3, mesh.end.x);
		ASSERT_EQ(3390e3 + 100e3, is.g->getMesh().end.y);
	}
}
