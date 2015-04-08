/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#pragma once
#include "Renderer.h"
#include "World.h"
#include "EventManager.h"

int main(int argc, char*argv[])
{
	EventManager::Initialize();
	Renderer::Initialize();

	World world;

	world.LoadScene();
	
	do
	{
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
