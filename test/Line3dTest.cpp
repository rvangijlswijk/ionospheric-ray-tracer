#include "gtest/gtest.h"
#include "../src/math/Line3d.h"
#include "../src/math/Vector3d.h"
#include "../src/math/Plane3d.h"

namespace {

	using namespace raytracer::math;

	class Line3dTest : public ::testing::Test {};

	TEST_F(Line3dTest, Intersect) {

		Plane3d plane = Plane3d(Vector3d(0, 1, 0), Vector3d(0, 10, 0));
		Line3d line = Line3d(Vector3d(0, 0, 0), Vector3d(0, 20, 0));

		Vector3d is = line.intersect(plane);

		ASSERT_EQ(0, is.x);
		ASSERT_EQ(10, is.y);
		ASSERT_EQ(0, is.z);

		plane = Plane3d(Vector3d(1, 1, 0), Vector3d(0, 10, 0));

		is = line.intersect(plane);

		ASSERT_EQ(0, is.x);
		ASSERT_EQ(10, is.y);
		ASSERT_EQ(0, is.z);

		plane = Plane3d(Vector3d(-1, 1, 0), Vector3d(5, 10, 0));
		line = Line3d(Vector3d(0, 0, 0), Vector3d(8, 20, 0));

		is = line.intersect(plane);

		ASSERT_NEAR(3.0 + 1.0/3.0, is.x, 1e-4);
		ASSERT_NEAR(8.0 + 1.0/3.0, is.y, 1e-4);
		ASSERT_EQ(0, is.z);

		plane = Plane3d(Vector3d(-1, 1, 0), Vector3d(0, 10, 0));
		plane.size = 2;
		line = Line3d(Vector3d(0, 0, 0), Vector3d(-40, 20, 0));

//		is = line.intersect(plane);
//
//		ASSERT_EQ(0, is.x);
//		ASSERT_EQ(0, is.y);
//		ASSERT_EQ(0, is.z);


	}
}
