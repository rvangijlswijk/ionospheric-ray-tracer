#include "gtest/gtest.h"
#include "../src/math/Matrix3d.h"
#include "../src/math/Vector3d.h"
#include "../src/math/Constants.h"
#include <cmath>

namespace {

	using namespace raytracer::math;

	class Matrix3dTest : public ::testing::Test {};

	TEST_F(Matrix3dTest, Rotation) {

		Matrix3d rotX = Matrix3d::createRotationMatrix(10.0 * Constants::PI / 180.0, Matrix3d::ROTATION_X);
		Matrix3d rotZ = Matrix3d::createRotationMatrix(30.0 * Constants::PI / 180.0, Matrix3d::ROTATION_Z);

		ASSERT_EQ(1, rotX.get(0,0));
		ASSERT_EQ(1, rotZ.get(2,2));

		ASSERT_NEAR(0.984, rotX.get(1,1), 1e-3);
		ASSERT_NEAR(-0.174, rotX.get(1,2), 1e-3);
		ASSERT_NEAR(0.867, rotZ.get(0,0), 1e-3);
		ASSERT_NEAR(0.5, rotZ.get(1,0), 1e-3);

		Matrix3d rotY = Matrix3d::createRotationMatrix(-15.0 * Constants::PI / 180.0, Matrix3d::ROTATION_Y);

		ASSERT_EQ(1, rotY.get(1,1));

		ASSERT_NEAR(-0.259, rotY.get(0,2), 1e-3);
		ASSERT_NEAR(0.259, rotY.get(2,0), 1e-3);
		ASSERT_NEAR(0.966, rotY.get(0,0), 1e-3);
	}

	TEST_F(Matrix3dTest, MatrixMultiplication) {

		Matrix3d rotX = Matrix3d::createRotationMatrix(15.0 * Constants::PI / 180.0, Matrix3d::ROTATION_X);
		Matrix3d rotZ = Matrix3d::createRotationMatrix(45.0 * Constants::PI / 180.0, Matrix3d::ROTATION_Z);

		Matrix3d rotationMatrix = rotX * rotZ;

		ASSERT_NEAR(0.707, rotationMatrix.get(0,0), 1e-3);
		ASSERT_EQ(0, rotX.get(1,0));
		ASSERT_NEAR(0.966, rotX.get(1,1), 1e-3);
		ASSERT_NEAR(-0.259, rotX.get(1,2), 1e-3);
		ASSERT_NEAR(0.707, rotZ.get(0,0), 1e-3);
		ASSERT_NEAR(-0.707, rotZ.get(0,1), 1e-3);
		ASSERT_EQ(0, rotZ.get(0,2));
		ASSERT_NEAR(0.683, rotationMatrix.get(1,0), 2e-2);
		ASSERT_NEAR(0, rotX.get(2,0), 1e-10);
		ASSERT_NEAR(0.259, rotX.get(2,1), 1e-3);
		ASSERT_NEAR(0.966, rotX.get(2,2), 1e-3);
		ASSERT_NEAR(-0.707, rotZ.get(0,1), 1e-3);
		ASSERT_NEAR(0.707, rotZ.get(1,1), 1e-3);
		ASSERT_NEAR(0, rotZ.get(2,1), 1e-10);
		ASSERT_NEAR(0.183, rotationMatrix.get(2,1), 1e-3);
		ASSERT_NEAR(0, rotationMatrix.get(0,2), 1e-3);

		// results should be inverted as the order of execution matters.
		Matrix3d rotationMatrix2 = rotZ * rotX;

		ASSERT_NEAR(0.707, rotationMatrix2.get(0,0), 1e-3);
		ASSERT_NEAR(0.707, rotationMatrix2.get(1,0), 1e-3);
		ASSERT_NEAR(0.259, rotationMatrix2.get(2,1), 1e-3);
		ASSERT_NEAR(0.183, rotationMatrix2.get(0,2), 1e-3);
	}

	TEST_F(Matrix3dTest, VectorMultiplication) {

		int radius = 3390e3;

		Matrix3d rotX = Matrix3d::createRotationMatrix(10.0 * Constants::PI / 180.0, Matrix3d::ROTATION_X);
		Matrix3d rotZ = Matrix3d::createRotationMatrix(-10.0 * Constants::PI / 180.0, Matrix3d::ROTATION_Z);
		Vector3d v = Vector3d(0, 4000, 0);

		Vector3d v2 = rotZ * v;
		Vector3d v3 = rotX * v2;

		ASSERT_NEAR(694.6, v2.x, 0.1);
		ASSERT_NEAR(3939.2, v2.y, 0.1);
		ASSERT_NEAR(0, v2.z, 1e-10);

		ASSERT_NEAR(694.6, v3.x, 0.1);
		ASSERT_NEAR(3879.4, v3.y, 0.1);
		ASSERT_NEAR(684, v3.z, 0.1);

		Matrix3d latitude = Matrix3d::createRotationMatrix(-5 * Constants::PI / 180.0, Matrix3d::ROTATION_X);
		Matrix3d longitude = Matrix3d::createRotationMatrix(0, Matrix3d::ROTATION_Z);
		Matrix3d rotationMatrix = latitude * longitude;

		ASSERT_NEAR(1, rotationMatrix.get(0,0), 1e-10);
		ASSERT_NEAR(0.996, rotationMatrix.get(1,1), 1e-3);

		Vector3d startPosition = rotationMatrix * Vector3d(0, radius, 0);

		ASSERT_NEAR(0, startPosition.x, 1e-3);
		ASSERT_NEAR(3.3771e6, startPosition.y, 1);
		ASSERT_NEAR(-2.954e5, startPosition.z, 1e2);
		ASSERT_NEAR(radius, startPosition.magnitude(), 1);
	}
}
