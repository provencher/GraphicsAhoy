//
// COMP 371 Assignment Framework
// Rita Branch
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Renderer.h"
#include "World.h"
#include "EventManager.h"
#include <string>
#include <iostream>

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
	/*
	std::vector<string> str;
	
	str.push_back("name     = \"Cube\"");
str.push_back("scaling  = 1.0 1.0 1.0");
str.push_back("position = 0.0 1.0 0.0");
str.push_back("rotation = 0.0 0.0 1.0 180");
str.push_back("boundpath = \"Path2\"");
str.push_back("pathspeed = 1.0");
	*/


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
