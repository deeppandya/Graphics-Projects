/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#include "Camera.h"
#include "EventManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>

using namespace glm;

Camera::Camera(glm::vec3 position): mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(0.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mAngularSpeed(2.5f) 
{
	arcballRotation = glm::mat4(1.0);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	arcballRotation = EventManager::GetArcBallRotation();
}

glm::mat4 Camera::GetViewMatrix() const
{
	// Calculate the View Matrix
	glm::mat4 viewMatrix(1.0f);
	
	//return viewMatrix;
	viewMatrix = glm::lookAt(	mPosition,
								glm::vec3(0.0f, 0.0f, 0.0f),
								glm::vec3(0.0f, 1.0f, 0.0f) );

	return viewMatrix * arcballRotation;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}