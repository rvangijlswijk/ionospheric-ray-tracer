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

			static Matrix3d createRotationMatrix(double angle, int direction);

			static int	ROTATION_X,
						ROTATION_Y,
						ROTATION_Z;

			double get(int i, int j);
			void set(int i, int j, double value);
			Matrix3d multiply(Matrix3d m2);
			Matrix3d operator*(Matrix3d m2) {
				return multiply(m2);
			}
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
