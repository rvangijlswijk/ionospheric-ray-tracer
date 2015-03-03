/*
 * Config.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: rian
 */

#include <iostream>
#include <cstdio>
#include <string.h>
#include "Config.h"
#include "../../contrib/rapidjson/filereadstream.h"

namespace raytracer {
namespace core {

	using namespace rapidjson;
	using namespace std;

	void Config::loadFromFile(const char * filepath) {

		FILE* fp = fopen(filepath, "r"); // non-Windows use "r"
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		_doc.ParseStream(is);

	}

	int Config::getInt(const char * path) {

		if (!_doc.HasMember(path)) {
			cerr << path << " doesnt exist!";
		}

		return _doc[path].GetInt();
	}

	double Config::getDouble(const char * path) {

		if (!_doc.HasMember(path)) {
			cerr << path << " doesnt exist!";
		}

		return _doc[path].GetDouble();
	}

} /* namespace core */
} /* namespace raytracer */
