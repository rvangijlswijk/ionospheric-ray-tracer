#include "gtest/gtest.h"
#include "../src/math/Vector2f.h"

namespace {

	class Vector2fTest : public ::testing::Test {
	};

	TEST_F(Vector2fTest, Magnitude) {

		::raytracer::math::Vector2f v;

		v.x = 4.0;
		v.y = 3.0;
		ASSERT_EQ(5.0, v.magnitude());
	}
}
