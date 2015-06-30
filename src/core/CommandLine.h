/*
 * CommandLine.h
 *
 *  Created on: 30 Jun 2015
 *      Author: rian
 */

#ifndef CORE_COMMANDLINE_H_
#define CORE_COMMANDLINE_H_

#include <stdlib.h>
#include <iostream>

namespace raytracer {
namespace core {

	class CommandLine {

		public:
			static CommandLine& getInstance() {
				static CommandLine instance;
				return instance;
			}
			void addToHeader(const char* msg);
			void updateBody(const char* msg);

		private:
			CommandLine() {}
			CommandLine(CommandLine const&);
			void operator = (CommandLine const&);
	};

} /* namespace core */
} /* namespace raytracer */

#endif /* CORE_COMMANDLINE_H_ */
