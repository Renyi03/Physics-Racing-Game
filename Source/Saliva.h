#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Box.h"

class ModuleGame;
class Snail;

class Saliva : public Box {
public:
	Saliva(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Snail* _owner = nullptr) :
		Box(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::DEFAULT, PhysicCategory::DEFAULT, PhysicGroup::LAND), owner(_owner) {
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
	void OnCollision(PhysBody* otherBody);		// New feature for handling collisions
	void EndCollision(PhysBody* otherBody);

protected:
	void SetTexture(Texture2D newTexture) override {
		Box::SetTexture(newTexture);
	}
	void ShrinkBody();							// Function to shrink the body 
	void ApplyFrictionToSnails();				// New function to apply friction

public:
	bool active = false;
	Texture2D texture;

private:
	float rotation = 0.0f;
	float timer = 0.0f;
	float lifeTime = 10.0f;
	int initialWidth;
	int initialHeight;
	float lastShrinkTime = 0.0f;
	float shrinkInterval = 0.3f;				// Shrink every 0.1 seconds

	// Friction
	float frictionCoefficient = 1.0f;			// How sticky is saliva (0.0 = no friction, 1.0 = very sticky)
	std::vector<Snail*> snailsInContact;		// Snails that are touching this saliva
	Snail* owner;								 

protected:
};