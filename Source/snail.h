#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Box.h"

class SnailAI;

enum class SnailType
{
	ENHYPEN,
	CHOPIN,
	ADO,
	MIKU
};
class Saliva;
class Snail : public Box {
public:
	Snail(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : 
		Box(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::SNAIL_CATEGORY, PhysicCategory::ALL) {
		mass = body->body->GetMass();
		body->ctype = ColliderType::SNAIL;
		originalMaskBits = body->body->GetFixtureList()->GetFilterData().maskBits;
	}
	virtual bool Start() {
		return true;
	}
	virtual bool CleanUp() {
		return true;
	}
	void Update() override;
	Vector2 GetPosition() const;
	Texture2D GetTexture() const { return texture; }
	void OnCollisionWithMap(PhysBody* mapObject);
	void SetAIInput(const b2Vec2& input) { aiInputDir = input; }
	b2Vec2 GetForward() const { return forward; }
	float GetSpeed() const
	{
		return body ? body->body->GetLinearVelocity().Length() : 0.0f;
	}

	//so snails race differently
	virtual void ApplySnailStats() {
		// Default stats (balanced)
		moveForce = 1.7f;
		rotation_base_rate = 3.0f;
		staticFrictionCoeff = 0.5f;
		dynamicFrictionCoeff = 0.3f;
	}


	void EndCollisionWithMap(PhysBody* mapObject);

protected:
	void Move();
	void Rotate(b2Vec2 inputDir, const b2Vec2& forward, bool has_input);
	void ApplyFriction(float i_staticFricion, float i_dynamicFriction);
	void ApplyLateralFriction(const b2Vec2& right);
	void Trail();
	void Hability();
	void SetTexture(Texture2D newTexture) override {
		Box::SetTexture(newTexture);
	}
	void SetMaskBits(uint16 newMaskBits);
	void RestoreMaskBits();
public:
	bool active = false;
	bool isAI = false;
	SnailAI* ai = nullptr;
private:
	float rotation = 0.0f;
	Saliva* saliva;
	std::vector < Saliva* > salives;
	b2Vec2 aiInputDir = b2Vec2(0.0f, 0.0f);
protected:
	Texture2D texture;
	Texture2D salivaTexture;
	//movement variables
	float moveForce = 1.7f;
	float mass = 1.0f;

	float staticFrictionCoeff = 0.5f;
	float dynamicFrictionCoeff = 0.3f;

	// Low-speed circular turning
	float low_speed_threshold = 2.0f; // speed below which tiny-push is applied
	float tiny_push_strength = 2.0f; // strength of the low-speed forward nudge (bigger circle 2-8, smaller circle 1-2)

	// drift 
	float lateralDamping = 0.2f; // how strongly sideways velocity is cancelled

	// Rotation tuning
	float rotation_base_rate = 3.0f; // base turn speed
	float rotation_speed_scale = 50.0f; // speed where turning is maxed out (needs to be adjusted but whatever)

	b2Vec2 forward;
	//trail
	std::vector<Vector2> trail;
	float trailTimer = 0.0f;
	float trailInterval = 0.05f; // drop a dot every 0.05 seconds

	//saliva
	std::vector<PhysBody*> salivaPhysBodies;
	float salivaTimer = 0.0f;
	float salivaInterval = 0.02f; // drop a rectangle every 0.02 seconds
	bool isSlobber = false;
	float slobberTimer = 0.0f;

	uint16 originalMaskBits;
};