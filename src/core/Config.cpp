/*
 * Config.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: rian
 */

#include "Config.h"
#include "../../contrib/rapidjson/filereadstream.h"
#include <cstdio>

namespace raytracer {
namespace core {

	using namespace rapidjson;

	void Config::loadFromFile(const char * filepath) {

		FILE* fp = fopen(filepath, "r"); // non-Windows use "r"
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		_doc.ParseStream(is);

	}

	int Config::getInt(const char * path) {

		return _doc[path].GetInt();
	}

	double Config::getDouble(const char * path) {

		return _doc[path].GetDouble();
	}

} /* namespace core */
} /* namespace raytracer */
