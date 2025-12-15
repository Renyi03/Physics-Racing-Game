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
	Saliva(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Snail* _owner) : 
		Box(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::DEFAULT, PhysicCategory::SNAIL_CATEGORY), owner(_owner) {
		body->SetSensor(true);
		body->ctype = ColliderType::SALIVA;
		active = true;
	}
	void Update() override;

protected:
	void SetTexture(Texture2D newTexture) override {
		Box::SetTexture(newTexture);
	}
public:
	bool active = false;
	Texture2D texture;
	Snail* owner;
private:
	float rotation = 0.0f;
	float timer = 0.0f;
protected:
	

};