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
	Snail(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::CAR, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
		body->body->ApplyForce(b2Vec2(0.0f, 100.f), body->body->GetWorldCenter(), true);
	}
	void Update();
private:
	void Move();
private:
	float speed = 2.0f;
	b2Vec2 velocity;
};