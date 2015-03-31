//============================================================================
// Name        : ConfigParser.h
// Author      : Rian van Gijlswijk
// Description : Pure abstract definition for reading custom config parameters.
//				 This defines how classes should implement parsing a config
//				 parameter.
//============================================================================

#ifndef SCENE_CONFIGPARSER_H_
#define SCENE_CONFIGPARSER_H_

#include "../../contrib/jsoncpp/value.h"
#include "../core/Application.h"

namespace raytracer {
namespace core {

	class ConfigParser {

		public:
			virtual Json::Value getValue(const char * path) = 0;
	};

} /* namespace scene */
} /* namespace raytracer */

#endif /* SCENE_IONOSPHERECONFIGPARSER_H_ */
