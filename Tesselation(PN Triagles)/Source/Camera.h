/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#pragma once
#include <GLM/glm.hpp>

class Camera
{
	public:
		Camera(glm::vec3 position);
		virtual ~Camera();

		virtual void Update(float dt);

		virtual glm::mat4 GetViewMatrix() const;
		virtual glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetViewProjectionMatrix() const;

	private:
		float mHorizontalAngle; // horizontal angle
		float mVerticalAngle;   // vertical angle

		float mSpeed;			// World units per second
		float mAngularSpeed;    // Degrees per pixel

		glm::vec3 mLookAt;
		glm::vec3 mPosition;

		glm::mat4 arcballRotation;

};
