/*	COMP 6761 Assignment 1 Framework
	Created by Kaustubha Mendhurwar on 09/09/14.
	Copyright (c) 2014 Concordia University. All rights reserved.
*/

#include "Model.h"
#include "Renderer.h"

using namespace std;

Model::Model() : mPosition(0.0f, 0.0f, 0.0f), mScaling(1.0f, 1.0f, 1.0f), mRotationAxis(1.0f, 1.0f, 0.0f), mRotationAngleInDegrees(0.0f)
{
	// Read our .obj file
	bool res = Renderer::LoadOBJ("../Source/Models/cube.obj", vertices, uvs, normals);
	
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);
	
	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);	
}

Model::~Model()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void Model::Update(float dt)
{
}

void Model::Draw()
{
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "W_Tran"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	
	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	0,				// attribute. No particular reason for 0, but must match the layout in the shader.
							3,				// size
							GL_FLOAT,		// type
							GL_FALSE,		// normalized?
							0, // stride
							(void*)0        // array buffer offset
						);

	// 2nd attribute buffer : vertex UVs
	/*glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	1,
							3,
							GL_FLOAT,
							GL_FALSE,
							0,
							(void*)0 
						);*/


	// 3rd attribute buffer : vertex Normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	1,
							3,
							GL_FLOAT,
							GL_FALSE,
							0,
							(void*)0
						);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Draw the triangles !
	glDrawArrays(GL_PATCHES, 0, vertices.size()); // 3 indices starting at 0 -> 1 triangle
	//glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 1 triangle
	//for(int i = 0; i < vertices.size(); i += 3)
		//glDrawArrays(GL_LINE_LOOP, i, 3);


	//glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);	
}

glm::mat4 Model::GetWorldMatrix() const
{
	glm::mat4 worldMatrix(1.0f);

	worldMatrix = glm::translate(worldMatrix, mPosition);
	worldMatrix = glm::rotate(worldMatrix, mRotationAngleInDegrees, mRotationAxis);
	worldMatrix = glm::scale(worldMatrix, mScaling);
	
	return worldMatrix;
}

void Model::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	mScaling = scaling;
}

void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	mRotationAxis = axis;
	mRotationAngleInDegrees = angleDegrees;
}
