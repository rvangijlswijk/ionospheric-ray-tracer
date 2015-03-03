//============================================================================
// Name        : NormalDistribution.h
// Author      : Rian van Gijlswijk
// Description : Return a normally distributed random number
//============================================================================

#ifndef NORMALDISTRIBUTION_H_
#define NORMALDISTRIBUTION_H_

#include <random>
#include <map>
#include <cmath>

namespace raytracer {
namespace math {

	using namespace std;

	typedef std::mt19937 Generator;

	class NormalDistribution {

		public:
			static NormalDistribution& getInstance() {
				static NormalDistribution instance;
				return instance;
			}
			double get(double mean, double stddev);

		private:
			NormalDistribution(){
				std::random_device rd;
				gen.seed(rd());
			}
			NormalDistribution(NormalDistribution const&);      // Don't Implement.
			void operator = (NormalDistribution const&); // Don't implement
			Generator gen;
	};

} /* namespace math */
} /* namespace raytracer */

#endif /* NORMALDISTRIBUTION_H_ */
