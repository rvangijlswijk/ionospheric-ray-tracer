#include "gtest/gtest.h"
#include "../src/math/Vector3d.h"

namespace {

	using namespace raytracer::math;

	class Vector3dTest : public ::testing::Test {};

	TEST_F(Vector3dTest, Magnitude) {

		Vector3d v;

		v.x = 4.0;
		v.y = 3.0;
		v.z = 6.0;
		ASSERT_NEAR(7.8102, v.magnitude(), 1e-3);
	}

	TEST_F(Vector3dTest, DotProduct) {

		Vector3d v1 = Vector3d(3, 2, 3);
		Vector3d v2 = Vector3d(6, 1, 2.5);

		ASSERT_NEAR(27.5, v1.dot(v2), 1e-4);

		Vector3d v3 = Vector3d(0, 2, 2.5);
		Vector3d v4 = Vector3d(1, -5, -2);

		ASSERT_NEAR(-15.0, v3.dot(v4), 1e-4);
	}

	TEST_F(Vector3dTest, CrossProduct) {

		Vector3d v1 = Vector3d(3, 2, 3);
		Vector3d v2 = Vector3d(6, 1, 2.5);

		Vector3d c1 = v1.cross(v2);
		ASSERT_NEAR(2, c1.x, 1e-4);
		ASSERT_NEAR(10.5, c1.y, 1e-4);
		ASSERT_NEAR(-9, c1.z, 1e-4);

		Vector3d v3 = Vector3d(0, 2, 2.5);
		Vector3d v4 = Vector3d(1, -5, -2);

		c1 = v3.cross(v4);
		ASSERT_NEAR(8.5, c1.x, 1e-4);
		ASSERT_NEAR(2.5, c1.y, 1e-4);
		ASSERT_NEAR(-2, c1.z, 1e-4);

	}

	TEST_F(Vector3dTest, Distance) {

		Vector3d v1 = Vector3d(3, 2, 3);
		Vector3d v2 = Vector3d(6, 1, 2.5);

		v1.distance(v2);

	}
}
