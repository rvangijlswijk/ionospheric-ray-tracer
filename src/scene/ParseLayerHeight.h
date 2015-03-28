//============================================================================
// Name        : ParseLayerHeight.h
// Author      : Rian van Gijlswijk
// Description : Read the layer height from a config file.
//============================================================================

#ifndef SCENE_PARSELAYERHEIGHT_H_
#define SCENE_PARSELAYERHEIGHT_H_

#include "../../contrib/jsoncpp/value.h"
#include "../core/Application.h"
#include "../math/Constants.h"
#include <math.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace raytracer {
namespace scene {

	class ParseLayerHeight {

		public:
			ParseLayerHeight();
			enum stratificationType {
				STRAT_CONSTANT,
				STRAT_CHAPMAN
			};
			int getDh(const char * stratificationType, double lowerHeight);
			int getDh(string stratificationType, double lowerHeight, double hMax);
		private:
			string getEnumType(int stratificationType);
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* SCENE_PARSELAYERHEIGHT_H_ */
