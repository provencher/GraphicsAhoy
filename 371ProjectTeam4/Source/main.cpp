#include "Renderer.h"
#include "World.h"
#include "EventManager.h"
#include <string>
#include <iostream>
//#include "SDL.h"

int main(int argc, char*argv[]){
	EventManager::Initialize();
	Renderer::Initialize();

	World world;

	if (argc > 1){
		world.LoadScene(argv[1]);
	} else {
		#if defined(PLATFORM_OSX)
			//world.LoadScene("Scenes/CoordinateSystem.scene");
			//world.LoadScene("Scenes/PathScene.scene");
			//world.LoadScene("Scenes/BSplineScene.scene");
			world.LoadScene("Scenes/ProjectSceen.scene");
		#else
			//world.LoadScene("../Scenes/CoordinateSystem.scene");
			//world.LoadScene("../Scenes/PathScene.scene");
			//world.LoadScene("../Scenes/BSplineScene.scene");
			world.LoadScene("../Scenes/ProjectSceen.scene");
		#endif
	}
	
	
	
	// Main Loop
	do {
		// Update Event Manager - Frame time / input / events processing 
		EventManager::Update();

		// Update World
		float dt = EventManager::GetFrameTime();
		world.Update(dt);

		// Draw World
		world.Draw();
	}
	while(EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();

	return 0;
}
