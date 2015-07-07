/*
 * Matrix3d.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: rian
 */

#include <cmath>
#include "Matrix3d.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

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

	/**
	 * Get an individual element in the matrix
	 */
	double Matrix3d::get(int i, int j) {

		return _matrix[i][j];
	}

	/**
	 * Set an individual element in the matrix
	 */
	void Matrix3d::set(int i, int j, double val) {

		_matrix[i][j] = val;
	}

	/**
	 * Create a rotation matrix around a certain axis, following the right-hand rule
	 * @param double angle: the angle in radians
	 */
	Matrix3d Matrix3d::createRotationMatrix(double angle, int axis) {

		Matrix3d m;

		if (axis == ROTATION_X) {
			m.set(0,0,1);
			m.set(0,1,0);
			m.set(0,2,0);
			m.set(1,0,0);
			m.set(1,1,cos(angle));
			m.set(1,2,-sin(angle));
			m.set(2,0,0);
			m.set(2,1,sin(angle));
			m.set(2,2,cos(angle));
		} else if (axis == ROTATION_Y) {
			m.set(0,0,cos(angle));
			m.set(0,1,0);
			m.set(0,2,sin(angle));
			m.set(1,0,0);
			m.set(1,1,1);
			m.set(1,2,0);
			m.set(2,0,-sin(angle));
			m.set(2,1,0);
			m.set(2,2,cos(angle));
		} else if(axis == ROTATION_Z) {
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

	/**
	 * Perform a full matrix multiplication.
	 * @param Matrix3d m2: the RHS matrix
	 */
	Matrix3d Matrix3d::multiply(Matrix3d rhs) {

		Matrix3d multipliedMatrix;

		for(int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				double val = 0;
				for (int k = 0; k < 3; k++) {
					BOOST_LOG_TRIVIAL(debug) << "(" << i << ", " << j << ", " << k << "): " <<  get(i,k) << " * " << rhs.get(k,j);
					val += get(i,k) * rhs.get(k,j);
				}
				BOOST_LOG_TRIVIAL(debug) << "(" << i << "," << j << "): " << val;
				multipliedMatrix.set(i, j, val);
			}
		}

		return multipliedMatrix;
	}

	/**
	 * Multiply this matrix with a vector
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
