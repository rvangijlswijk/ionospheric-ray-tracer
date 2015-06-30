#include "gtest/gtest.h"
#include "../src/math/Vector2d.h"
#include "../src/math/Constants.h"

namespace {

	using namespace ::raytracer::math;

	class Vector2dTest : public ::testing::Test {
	};

	TEST_F(Vector2dTest, Magnitude) {

		Vector2d v;

		v.x = 4.0;
		v.y = 3.0;
		ASSERT_EQ(5.0, v.magnitude());
	}

	TEST_F(Vector2dTest, DotProduct) {

		Vector2d v, v2;

		v.x = 4.0;
		v.y = 3.0;

		v2.x = 1.0;
		v2.y = 5.0;

		ASSERT_EQ(19, v.dotProduct(v2));
	}

	TEST_F(Vector2dTest, Angle) {

		Vector2d v, v2, v3, v4;

		v.x = 4.0;
		v.y = 3.0;

		v2.x = 1.0;
		v2.y = 5.0;

		v3.x = 5.0;
		v3.y = 1.0;

		v4.x = 1.0;
		v4.y = -5.0;

		ASSERT_NEAR(0.7299, v.angle(v2), 1e-5);
		ASSERT_NEAR(Constants::PI/2, v3.angle(v4), 5e-5);
	}
}
