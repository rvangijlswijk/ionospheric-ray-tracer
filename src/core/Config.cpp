/*
 * Config.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: rian
 */

#include <iostream>
#include <fstream>
#include "Config.h"
#include <string>
#include "../../contrib/jsoncpp/reader.h"

namespace raytracer {
namespace core {

	using namespace std;

	Config::Config() {}

	Config::Config(const char * filepath) {

		loadFromFile(filepath);
	}

	void Config::loadFromFile(const char * filepath) {

		Json::Reader r = Json::Reader();
		std::ifstream test(filepath, std::ifstream::binary);
		bool success = r.parse(test, _doc, false);
		if ( !success ) {
			// report to the user the failure and their locations in the document.
			std::cout  << r.getFormatedErrorMessages() << "\n";
		}
	}

	int Config::getInt(const char * path) {

		if (!_doc.isMember(path)) {
			cerr << path << " does not exist!" << endl;
		}

		return _doc.get(path, "UTF-32").asInt();
	}

	double Config::getDouble(const char * path) {

		if (!_doc.isMember(path)) {
			cerr << path << " does not exist!" << endl;
		}

		return _doc.get(path, "UTF-32").asDouble();
	}

	Json::Value Config::getArray(const char * path) {

		if (!_doc.isMember(path)) {
			cerr << path << " does not exist!" << endl;
		}

		if (!_doc.get(path, "UTF-32").isArray()) {
			cerr << path << " is not an array!" << endl;
		}

		return _doc[path];
	}

	Json::Value Config::getObject(const char * path) {

		if (!_doc.isMember(path)) {
			cerr << path << " does not exist!" << endl;
		}

		if (!_doc.get(path, "UTF-32").isObject()) {
			cerr << path << " is not an object!" << endl;
		}

		return _doc[path];
	}

} /* namespace core */
} /* namespace raytracer */
