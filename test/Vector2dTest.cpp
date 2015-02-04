#include "gtest/gtest.h"
#include "../src/math/Vector2d.h"

namespace {

	class Vector2dTest : public ::testing::Test {
	};

	TEST_F(Vector2dTest, Magnitude) {

		::raytracer::math::Vector2d v;

		v.x = 4.0;
		v.y = 3.0;
		ASSERT_EQ(5.0, v.magnitude());
	}
}
