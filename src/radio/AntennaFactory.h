/*
 * AntennaFactory.h
 *
 *  Created on: 27 Jul 2015
 *      Author: rian
 */

#ifndef RADIO_ANTENNAFACTORY_H_
#define RADIO_ANTENNAFACTORY_H_

#include <iostream>
#include <string>
#include <map>
#include "IAntenna.h"

namespace raytracer {
namespace radio {

	using namespace std;

	template<typename T> IAntenna * createT() { return new T; }

	class AntennaFactory {

		public:
			typedef std::map<std::string, IAntenna*(*)()> map_type;

			static IAntenna * createInstance(std::string const& s) {
				map_type::iterator it = getMap()->find(s);
				if(it == getMap()->end()) {
					std::cerr << "Instance type " << s << " does not exist!";
					std::exit(-1);
				}

				return it->second();
			}

			static void printMappedTypes() {

				for (map_type::iterator it=getMap()->begin(); it!=getMap()->end(); ++it) {
					std::cout << it->first << " => " << it->second << '\n';
				}
			}

		protected:
			static map_type * getMap() {
				// never delete'ed. (exist until program termination)
				// because we can't guarantee correct destruction order
				if(!map) { map = new map_type; }
				return map;
			}

		private:
			static map_type * map;
	};

	template<typename T>
	class AntennaRegister : AntennaFactory {
		public:
			AntennaRegister(std::string const& s) {
				getMap()->insert(std::pair<std::string, IAntenna*(*)()>(s, &createT<T>));
			}
	};

} /* namespace radio */
} /* namespace raytracer */

#endif /* RADIO_ANTENNAFACTORY_H_ */
