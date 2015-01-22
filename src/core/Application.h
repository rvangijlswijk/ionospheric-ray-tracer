//============================================================================
// Name        : Application.h
// Author      : Rian van Gijlswijk
// Description : Main application file. Responsible for starting, running and
//				 stopping the simulation
//============================================================================

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "../scene/SceneManager.h"

namespace raytracer {
namespace core {

	using namespace scene;

	class Application {

		public:
			static Application& getInstance() {
				static Application instance;
				return instance;
			}
			void init();
			void start();
			void run();
			void stop();
			SceneManager getSceneManager();

		private:
			Application() {
				isRunning = false;
			}
			Application(Application const&);      // Don't Implement.
			void operator = (Application const&); // Don't implement
			void createScene();
			bool isRunning;
			SceneManager scm;

	};

} /* namespace core */
} /* namespace raytracer */

#endif /* APPLICATION_H_ */
