#include "gtest/gtest.h"
#include "../src/tracer/Ray.h"
#include "../src/math/Constants.h"

namespace {

	using namespace ::raytracer::tracer;
	using namespace ::raytracer::math;

	class RayTest : public ::testing::Test {

		protected:
			void SetUp() {

				r.o = Vector2f(1,1);
				r.d = Vector2f(1,1.7320);
				r.frequency = 2.5e6;
			}

			Ray r;
	};

	TEST_F(RayTest, GetAngle) {

		ASSERT_NEAR(1.047, r.getAngle(), 0.001);

		Ray r2 = Ray();
		r2.d = Vector2f(1, -1.732);

		ASSERT_NEAR(-1.047, r2.getAngle(), 0.001);

		Ray r3 = Ray();
		r3.d = Vector2f(-10, 10);

		ASSERT_NEAR(2.356, r3.getAngle(), 0.001);
	}

	TEST_F(RayTest, GetNormalAngle) {

		ASSERT_NEAR(0.524, r.getNormalAngle(), 0.001);
	}

	TEST_F(RayTest, SetNormalAngle) {

		r.setNormalAngle(50.0*Constants::PI/180);
		ASSERT_NEAR(0.766, r.d.x, 0.01);
		ASSERT_NEAR(0.643, r.d.y, 0.01);
		ASSERT_NEAR(0.873, r.getNormalAngle(), 0.001);

		// invert, point nadir with offset of pi/6 rad
		r.setNormalAngle(130.0*Constants::PI/180);
		ASSERT_NEAR(0.766, r.d.x, 0.01);
		ASSERT_NEAR(-0.643, r.d.y, 0.01);
		ASSERT_NEAR(2.269, r.getNormalAngle(), 0.001);

		// point nadir with offset of -pi/6 rad
		r.setNormalAngle(230.0*Constants::PI/180);
		ASSERT_NEAR(-0.766, r.d.x, 0.01);
		ASSERT_NEAR(-0.643, r.d.y, 0.01);
		ASSERT_NEAR(4.014, r.getNormalAngle(), 0.001);

		// zenith with offset of -pi/6 rad
		r.setNormalAngle(-50.0*Constants::PI/180);
		ASSERT_NEAR(-0.766, r.d.x, 0.01);
		ASSERT_NEAR(0.643, r.d.y, 0.01);
		ASSERT_NEAR(-0.872, r.getNormalAngle(), 0.001);
	}
}
