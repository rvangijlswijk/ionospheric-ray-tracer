//============================================================================
// Name        : Config.h
// Author      : Rian van Gijlswijk
// Description : Loads and stores simulation parameters from json config files
//============================================================================

#ifndef CONFIG_H_
#define CONFIG_H_

#include "../../contrib/rapidjson/document.h"

namespace raytracer {
namespace core {

	using namespace rapidjson;

	class Config {

		public:
			static Config& getInstance() {
				static Config instance;
				return instance;
			}
			void loadFromFile(const char * filepath);
			int getInt(const char * path);
			double getDouble(const char * path);

		private:
			Config() {
			}
			Config(Config const&);      // Don't Implement.
			void operator = (Config const&); // Don't implement
			Document _doc;

	};

} /* namespace core */
} /* namespace raytracer */

#endif /* CONFIG_H_ */
