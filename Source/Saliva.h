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
	Saliva(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) :
		Box(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::DEFAULT, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
		body->SetSensor(true);
		active = true;
		initialWidth = 26;
		initialHeight = 43;
	}
	~Saliva();
	bool Start();
	virtual bool CleanUp() {
		return true;
	}
	void Update() override;
	bool IsExpired() const;
	float GetAlpha() const;

protected:
	void SetTexture(Texture2D newTexture) override {
		Box::SetTexture(newTexture);
	}
	void ShrinkBody(); // Function to shrink the body 

public:
	bool active = false;
	Texture2D texture;

private:
	float rotation = 0.0f;
	float timer = 0.0f;
	float lifeTime = 2.0f;
	int initialWidth;
	int initialHeight;
	float lastShrinkTime = 0.0f;
	float shrinkInterval = 0.1f; // Shrink every 0.1 seconds

protected:
};