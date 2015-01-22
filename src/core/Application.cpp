//============================================================================
// Name        : Application.cpp
// Author      : Rian van Gijlswijk
// Description : Main application file.
//============================================================================

#include <iostream>
#include <list>
#include "Application.h"
#include "../tracer/Ray.h"
#include "../exporter/MatlabExporter.h"

namespace raytracer {
namespace core {

	using namespace std;
	using namespace tracer;
	using namespace exporter;

	list<Vector2f> rayPath;

	Application::Application() {

		start();
		run();
	}

	void Application::start() {

		isRunning = true;
	}

	void Application::run() {

		// trace a ray
		Ray r;
		r.o = Vector2f(0,0);
		r.d = Vector2f(1,1.94);

		r.trace(rayPath);

		MatlabExporter me;
		me.dump("data.dat", rayPath);

		stop();
	}

	void Application::stop() {

		isRunning = false;
	}

} /* namespace core */
} /* namespace raytracer */


int main() {

	raytracer::core::Application app;

}
