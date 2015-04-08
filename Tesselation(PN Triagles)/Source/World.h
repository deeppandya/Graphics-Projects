/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#pragma once
#include "Model.h"
#include <vector>
#include "Camera.h"


class World
{
public:
	World();
	~World();
	
	void Update(float dt);
	void Draw();
	void LoadScene();

private:
	Model* mModel;
	Camera* mCamera;	
};
