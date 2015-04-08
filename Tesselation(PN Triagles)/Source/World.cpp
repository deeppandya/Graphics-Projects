/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#include "Model.h"
#include "World.h"
#include "Renderer.h"
#include "Camera.h"
#include "EventManager.h"

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

float tesslevelinner=1;
float tesslevelouter=1;

World::World()
{
	// Setup Camera
	mCamera = new Camera(vec3(0.0f, 0.0f, 10.0f));	
}

World::~World()
{
	// Models
	mModel->~Model();

	// Camera
	mCamera->~Camera();
}
	
void World::Update(float dt)
{
	// Update current Camera
	mCamera->Update(dt);

	// Update models
	mModel->Update(dt);	
}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set Shader... In a more sofisticated application, each model could use a different shader
	// In our case, all the models use a common shader
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint Float01 = glGetUniformLocation(Renderer::GetShaderProgramID(), "TessLevelInner"); 
	GLuint Float02 = glGetUniformLocation(Renderer::GetShaderProgramID(), "TessLevelOuter"); 
	glUniform1f(Float01,tesslevelinner);
	glUniform1f(Float02,tesslevelouter);
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "VP"); 

	// Send the view projection constants to the shader
	mat4 VP = mCamera->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
	GLuint VPviewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "viewMatrix"); 

	// Send the view projection constants to the shader
	mat4 VPview = mCamera->GetViewMatrix();
	glUniformMatrix4fv(VPviewMatrixLocation, 1, GL_FALSE, &VPview[0][0]);	
	// Draw model
	mModel->Draw();

	Renderer::EndFrame();
}

void World::LoadScene()
{
	mModel = new Model();
}
