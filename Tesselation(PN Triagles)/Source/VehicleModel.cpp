//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "VehicleModel.h"
#include "CubeModel.h"


using namespace std;
using namespace glm;

VehicleModel::VehicleModel()
{
	mBody = new CubeModel(vec3(1.0f, 1.0f, 4.0f), this);
	mBody->SetPosition(vec3(0.0f, 1.5f, 0.0f));
	mBody->SetRotation(vec3(1.0f, 0.0f, 0.0f), -20.0f);

	mTail = new CubeModel(vec3(0.1f, 1.5f, 1.0f), mBody);
	mTail->SetPosition(vec3(0.0f, 0.75f, -2.0f));
	mTail->SetRotation(vec3(1.0f, 0.0f, 0.0f), -15.0f);

	mWing = new CubeModel(vec3(6.0f, 0.1f, 1.0f), mBody);

	mLeftRotor = new CubeModel(vec3(0.1f, 1.75f, 0.1f), mWing);
	mLeftRotor->SetPosition(vec3(-2.0f, 0.0f, 0.55f));

	mRightRotor = new CubeModel(vec3(0.1f, 1.75f, 0.1f), mWing);
	mRightRotor->SetPosition(vec3( 2.0f, 0.0f, 0.55f));

	mNose = new CubeModel(vec3(0.2f, 0.2f, 0.2f), mBody);
	mNose->SetPosition(vec3(0.0f, 0.0f, 2.2f));
	mNose->SetRotation(vec3(1.0f, 0.0f, 0.0f), 180);
}

VehicleModel::~VehicleModel()
{
}

void VehicleModel::Update(float dt)
{	
	// Both rotors spin in sync
	float rotationAngle = mLeftRotor->GetRotationAngle() + 1080.0f * dt;
	if (rotationAngle > 360)
	{
		rotationAngle -= 360;
	}

	mLeftRotor->SetRotation(vec3(0.0f, 0.0f, 1.0f), rotationAngle);
	mRightRotor->SetRotation(vec3(0.0f, 0.0f, 1.0f), rotationAngle);
}

void VehicleModel::Draw()
{
	mBody->Draw();
	mWing->Draw();
	mTail->Draw();
	mLeftRotor->Draw();
	mRightRotor->Draw();
	mNose->Draw();
}

bool VehicleModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
