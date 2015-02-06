//============================================================================
// Name        : NDouble.h
// Author      : Rian van Gijlswijk
// Description : Nullable double
//============================================================================

#ifndef NDOUBLE_H_
#define NDOUBLE_H_

namespace raytracer {
namespace math {

	class NDouble {

		public:
			NDouble();
			void set(double v);
			double get();
			bool isset();
			void unset();

		private:
			double val = 0.0;
			bool hasBeenSet;
	};

} /* namespace threading */
} /* namespace raytracer */

#endif /* NDOUBLE_H_ */
