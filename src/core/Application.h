//============================================================================
// Name        : Application.h
// Author      : Rian van Gijlswijk
// Description : Main application file. Responsible for starting, running and
//				 stopping the simulation
//============================================================================

#ifndef APPLICATION_H_
#define APPLICATION_H_

namespace raytracer {
namespace core {

class Application {

	public:
		Application();
		void start();
		void run();
		void stop();

	private:
		bool isRunning = false;

};

} /* namespace core */
} /* namespace raytracer */

#endif /* APPLICATION_H_ */
