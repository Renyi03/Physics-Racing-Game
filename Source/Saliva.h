#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Box.h"

class ModuleGame;

class Saliva : public Box {
public:
	Saliva(ModulePhysics* physics, int _x, int _y, Module* _listener) : 
		Box(physics, _x, _y, 26, 43, _listener, texture, PhysicCategory::DEFAULT, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
		body->SetSensor(true);
		active = true;
		texture = LoadTexture("Assets/Textures/Chopin_Snail.png");
	}
	bool Start();
	virtual bool CleanUp() {
		return true;
	}
	void Update() override;

protected:
	void SetTexture(Texture2D newTexture) override {
		Box::SetTexture(newTexture);
	}
public:
	bool active = false;
private:
	float rotation = 0.0f;
	float timer = 0.0f;
protected:

};