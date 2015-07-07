//============================================================================
// Name        : Matrix3d.h
// Author      : Rian van Gijlswijk
// Description : A 3x3 matrix representation
//============================================================================

#ifndef MATH_MATRIX3D_H_
#define MATH_MATRIX3D_H_

#include "Vector3d.h"

namespace raytracer {
namespace math {

	class Matrix3d {

		public:
			Matrix3d();
			Matrix3d(Vector3d xPos, Vector3d yPos, Vector3d zPos);

			/**
			 * Create a rotation matrix around a certain axis, following the right-hand rule
			 * @param double angle: the angle in radians
			 */
			static Matrix3d createRotationMatrix(double angle, int axis);

			static int	ROTATION_X,
						ROTATION_Y,
						ROTATION_Z;

			/**
			 * Get an individual element in the matrix
			 */
			double get(int i, int j);

			/**
			 * Set an individual element in the matrix
			 */
			void set(int i, int j, double value);

			/**
			 * Perform a full matrix multiplication.
			 * @param Matrix3d m2: the RHS matrix
			 */
			Matrix3d multiply(Matrix3d m2);
			Matrix3d operator*(Matrix3d m2) {
				return multiply(m2);
			}

			/**
			 * Multiply this matrix with a vector
			 */
			Vector3d multiply(Vector3d vin);
			Vector3d operator*(Vector3d vin) {
				return multiply(vin);
			}

		private:
			/**
			 * The internal 3x3 matrix, represented in row,col format
			 */
			double _matrix[3][3];
	};

} /* namespace threading */
} /* namespace raytracer */

#endif /* MATH_MATRIX3D_H_ */
