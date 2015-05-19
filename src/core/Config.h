//============================================================================
// Name        : Config.h
// Author      : Rian van Gijlswijk
// Description : Loads and stores simulation parameters from json config files
//============================================================================

#ifndef CONFIG_H_
#define CONFIG_H_

#include "../../contrib/jsoncpp/value.h"

namespace raytracer {
namespace core {

	class Config {

		public:
			Config();
			Config(const char * filepath);
			void loadFromFile(const char * filepath);
			int getInt(const char * path);
			double getDouble(const char * path);
			Json::Value getArray(const char * path);
			Json::Value getObject(const char * path);

			friend std::ostream& operator<<(std::ostream &strm, const raytracer::core::Config &c) {

				return strm << c._doc.toStyledString();
			}

		private:
			Json::Value _doc;

	};

} /* namespace core */
} /* namespace raytracer */

#endif /* CONFIG_H_ */
