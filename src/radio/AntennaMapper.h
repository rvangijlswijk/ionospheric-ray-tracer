//============================================================================
// Name        : AntennaMapper.h
// Author      : Rian van Gijlswijk
// Description : Keeps track of all registered antenna types
//============================================================================

#ifndef RADIO_ANTENNAMAPPER_H_
#define RADIO_ANTENNAMAPPER_H_

#include "IAntenna.h"

namespace raytracer {
namespace radio {

	using namespace std;

	typedef std::map<std::string, IAntenna*(*)> map_type;

	class AntennaMapper {

		public:
			AntennaMapper();

			/**
			 * Register an IAntenna class in the map
			 */
			static void reg(const char * name);

			/**
			 * Retrieve an IAntenna class from the map
			 */
			static IAntenna* get(const char * name);

		private:
			/**
			 * A map containing all registered IAntenna classes
			 */
			map_type _map;
	};

} /* namespace radio */
} /* namespace raytracer */

#endif /* RADIO_ANTENNAMAPPER_H_ */
