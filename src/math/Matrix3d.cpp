/*
 * Matrix3d.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: rian
 */

#include <cmath>
#include "Matrix3d.h"

namespace raytracer {
namespace math {

	Matrix3d::Matrix3d() {}

	Matrix3d::Matrix3d(Vector3d col1, Vector3d col2, Vector3d col3) {

		// row, col
		_matrix[0][0] = col1.x;
		_matrix[0][1] = col1.y;
		_matrix[0][2] = col1.z;
		_matrix[1][0] = col2.x;
		_matrix[1][1] = col2.y;
		_matrix[1][2] = col2.z;
		_matrix[2][0] = col3.x;
		_matrix[2][1] = col3.y;
		_matrix[2][2] = col3.z;
	}

	double Matrix3d::get(int i, int j) {

		return _matrix[i][j];
	}

	void Matrix3d::set(int i, int j, double val) {

		_matrix[i][j] = val;
	}

	Matrix3d Matrix3d::createRotationMatrix(double angle, int direction) {

		Matrix3d m;

		if (direction == ROTATION_X) {
			m.set(0,0,1);
			m.set(0,1,0);
			m.set(0,2,0);
			m.set(1,0,0);
			m.set(1,1,cos(angle));
			m.set(1,2,-sin(angle));
			m.set(2,0,0);
			m.set(2,1,sin(angle));
			m.set(2,2,cos(angle));
		} else if (direction == ROTATION_Y) {
			m.set(0,0,cos(angle));
			m.set(0,1,0);
			m.set(0,2,sin(angle));
			m.set(1,0,0);
			m.set(1,1,1);
			m.set(1,2,0);
			m.set(2,0,-sin(angle));
			m.set(2,1,0);
			m.set(2,2,cos(angle));
		} else if(direction == ROTATION_Z) {
			m.set(0,0,cos(angle));
			m.set(0,1,-sin(angle));
			m.set(0,2,0);
			m.set(1,0,sin(angle));
			m.set(1,1,cos(angle));
			m.set(1,2,0);
			m.set(2,0,0);
			m.set(2,1,0);
			m.set(2,2,1);
		}

		return m;
	}

	Matrix3d Matrix3d::multiply(Matrix3d rhs) {

		return Matrix3d();
	}

	/**
	 * Multiply with a matrix, to perform rotations for example
	 */
	Vector3d Matrix3d::multiply(Vector3d vin) {

		Vector3d newVector = Vector3d();
		newVector.x = vin.x * get(0,0) + vin.y * get(0,1) + vin.z * get(0,2);
		newVector.y = vin.x * get(1,0) + vin.y * get(1,1) + vin.z * get(1,2);
		newVector.z = vin.x * get(2,0) + vin.y * get(2,1) + vin.z * get(2,2);

		return newVector;
	}

	int Matrix3d::ROTATION_X = 0;
	int Matrix3d::ROTATION_Y = 1;
	int Matrix3d::ROTATION_Z = 2;

} /* namespace threading */
} /* namespace raytracer */
