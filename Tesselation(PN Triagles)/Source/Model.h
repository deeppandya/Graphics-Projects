/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#pragma once
// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

class Model
{
public:
		Model();
		~Model();

		void Update(float dt);
		void Draw();

		glm::mat4 GetWorldMatrix() const;

		void SetPosition(glm::vec3 position);
		void SetScaling(glm::vec3 scaling);
		void SetRotation(glm::vec3 axis, float angleDegrees);

		glm::vec3 GetPosition() const		{ return mPosition; }
		glm::vec3 GetScaling() const		{ return mScaling; }
		glm::vec3 GetRotationAxis() const	{ return mRotationAxis; }
		float     GetRotationAngle() const	{ return mRotationAngleInDegrees; }

protected:
		//std::vector<glm::vec3> vertices;
		//std::vector<glm::vec2> uvs;	// Won't be used at the moment.
		//std::vector<glm::vec3> normals; // Won't be used at the moment.
		glm::vec3 mPosition;
		glm::vec3 mScaling;
		glm::vec3 mRotationAxis;
		float     mRotationAngleInDegrees;

private:
		unsigned int mVertexBufferID;
		unsigned int mVertexArrayID;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals; // Won't be used at the moment.
};
