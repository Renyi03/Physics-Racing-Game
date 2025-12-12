#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Box.h"

class Snail : public Box {
public:
	Snail(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : 
		Box(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::CAR, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
		mass = body->body->GetMass();
	}
	void Start();
	void Update() override;
protected:
	void Move();
	void Rotate(b2Vec2 inputDir);
	void ApplyFriction(float i_staticFricion, float i_dynamicFriction);
	void ApplyLateralFriction();
	void Trail();
private:
	float rotation;
	Vector2 GetPosition() const;
protected:
	b2Vec2 forward;
	//trail
	std::vector<Vector2> trail;
	float trailTimer = 0.0f;
	float trailInterval = 0.05f; // drop a dot every 0.05 seconds

	float moveForce = 1.7f;
	float mass;
	float staticFrictionCoeff = 0.5f;
	float dynamicFrictionCoeff = 0.3f;
};