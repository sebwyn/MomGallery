#include "Game.hpp"

#include "Time.hpp"
#include "Camera.hpp"
#include "CollideableManager.hpp"

using namespace Engine;

void Game::run(){
	init();	
	start();

	mainloop();

	finish();
	cleanup();
}

void Game::init(){
	win = new Window("Window", 1024, 768);
	root = new GameObject("root", true);
	
	camera = new GameObject("camera", true);
	camera->attachComponent(new Camera(1024, 768));
	root->addChild(camera);
	
	Time::start();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

	std::cout << glGetString(GL_VERSION) << std::endl;
}

void Game::cleanup(){
	delete win;
	delete camera;
	delete root;
}

void Game::mainloop(){
	double previousTime = glfwGetTime();
	double pTime = previousTime;
	int frameCount = 0;

	while(!win->closed()){
		double currentTime = glfwGetTime();
		if (currentTime-previousTime > 0.016){
			frameCount++;
			if ( currentTime - pTime >= 1.0 )
			{
				std::cout << frameCount << std::endl;
				frameCount = 0;
				pTime = glfwGetTime();
			}
			
			Time::update();

			Camera* mainCam = (Camera*)camera->getComponent("camera");
			mainCam->updateAspect(win->getWidth(), win->getHeight());
			win->clear();

			CollideableManager::update();

			root->update();
			root->lateUpdate();

			root->render(mainCam->getProjectionMatrix(), mainCam->getViewMatrix());

			win->update();
			previousTime = currentTime;
		}
	}
}
