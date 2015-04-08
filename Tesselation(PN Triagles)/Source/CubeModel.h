//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"

class CubeModel : public Model
{
public:
	// Solution for parenting, delete me
	CubeModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), const Model* parent = NULL);
	virtual ~CubeModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int mVertexBufferID;
	unsigned int mVertexArrayID;
};
