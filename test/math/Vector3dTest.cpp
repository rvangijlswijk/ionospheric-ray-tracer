#include "gtest/gtest.h"
#include "../../src/math/Vector3d.h"
#include "../../src/math/Constants.h"
#include <cmath>

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
		ASSERT_NEAR(27.5, v1 * v2, 1e-4);

		Vector3d v3 = Vector3d(0, 2, 2.5);
		Vector3d v4 = Vector3d(1, -5, -2);

		ASSERT_NEAR(-15.0, v3.dot(v4), 1e-4);
		ASSERT_NEAR(-15.0, v3 * v4, 1e-4);

		Vector3d v5 = Vector3d(0.174, 0.985, 0);
		Vector3d v6 = Vector3d(0.643, 0.767, 0);

		ASSERT_NEAR(0.8674, v5.dot(v6), 1e-4);
		ASSERT_NEAR(0.8674, v5 * v6, 1e-4);
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

	TEST_F(Vector3dTest, Norm) {

		Vector3d v1 = Vector3d(3, 2, 3);
		Vector3d v2 = Vector3d(6, 1, 2.5);

		ASSERT_NEAR(3 / sqrt(22), v1.norm().x, 1e-4);
		ASSERT_NEAR(2 / sqrt(22), v1.norm().y, 1e-4);
		ASSERT_NEAR(3 / sqrt(22), v1.norm().z, 1e-4);
		ASSERT_NEAR(6 / sqrt(43.25), v2.norm().x, 1e-4);
		ASSERT_NEAR(1 / sqrt(43.25), v2.norm().y, 1e-4);
		ASSERT_NEAR(2.5 / sqrt(43.25), v2.norm().z, 1e-4);
	}

	TEST_F(Vector3dTest, Angle) {

		Vector3d v1 = Vector3d(1, 0, 0);
		Vector3d v2 = Vector3d(0, 1, 0);

		ASSERT_NEAR(Constants::PI/2, v1.angle(v2), 1e-4);

		Vector3d v3 = Vector3d(0.174, 0.985, 0);
		Vector3d v4 = Vector3d(0.643, 0.767, 0);

		ASSERT_NEAR(0.5228, v3.angle(v4), 1e-4);

		Vector3d v5 = Vector3d(0.560, 0.579, 0);

		ASSERT_NEAR(0.5939, v5.angle(v3), 1e-4);
	}
}
