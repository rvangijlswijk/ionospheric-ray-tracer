//============================================================================
// Name        : ParseLayerHeight.h
// Author      : Rian van Gijlswijk
// Description : Read the layer height from a config file.
//============================================================================

#ifndef SCENE_IONOSPHERECONFIGPARSER_H_
#define SCENE_IONOSPHERECONFIGPARSER_H_

#include "../core/Application.h"
#include "../core/ConfigParser.h"
#include "../math/Constants.h"
#include <math.h>

namespace raytracer {
namespace scene {

	using namespace core;

	class IonosphereConfigParser : public ConfigParser {

		public:
			IonosphereConfigParser();
			Json::Value getValue(const char * path);
			int getDh(const char * stratificationType, double lowerHeight);
			int getDh(const char * stratificationType, double lowerHeight, double hMax);
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* SCENE_IONOSPHERECONFIGPARSER_H_ */
