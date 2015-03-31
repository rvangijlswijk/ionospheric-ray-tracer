/*
 * IonosphereConfigParser.cpp
 *
 *  Created on: 27 Mar 2015
 *      Author: rian
 */

#include "IonosphereConfigParser.h"

namespace raytracer {
namespace scene {

	using namespace std;
	using namespace core;
	using namespace math;

	IonosphereConfigParser::IonosphereConfigParser() {}

	Json::Value IonosphereConfigParser::getValue(const char * path) {

		return Application::getInstance().getApplicationConfig().getObject(path);
	}

	int IonosphereConfigParser::getDh(const char * stratificationType, double lowerHeight, double hMax) {

		const Json::Value layerHeight = getValue("layerHeight");
		if (layerHeight.isMember(stratificationType)) {
			if (stratificationType == "chapman") {
				int dhnmin = layerHeight[stratificationType].get("dhnmin", 0).asInt();
				int dhnmax = layerHeight[stratificationType].get("dhnmax", 0).asInt();
				double z = (lowerHeight - hMax) / Constants::NEUTRAL_SCALE_HEIGHT;
				double dh = dhnmin + dhnmax * (1 - exp(1 - z - exp(-z)));
				return (int)round(dh);
			} else {
				BOOST_LOG_TRIVIAL(error) << stratificationType << " wrong function!";
			}
		} else {
			BOOST_LOG_TRIVIAL(error) << stratificationType << " not found!";
		}

	}

	int IonosphereConfigParser::getDh(const char * stratificationType, double lowerHeight) {

		const Json::Value layerHeight = Application::getInstance().getApplicationConfig().getObject("layerHeight");
		if (layerHeight.isMember(stratificationType)) {
			if (stratificationType == "constant") {
				return layerHeight[stratificationType].asInt();
			} else {
				BOOST_LOG_TRIVIAL(error) << stratificationType << " wrong function!";
			}
		} else {
			BOOST_LOG_TRIVIAL(error) << stratificationType << " not found!";
		}

	}

} /* namespace scene */
} /* namespace raytracer */
