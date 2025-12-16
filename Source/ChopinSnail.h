#pragma once
#include "Box.h"
#include "Snail.h"

class ChopinSnail : public Snail {
public:
	ChopinSnail(ModulePhysics* physics, int _x, int _y, Module* _listener)
		: Snail(physics, _x, _y, _listener, Texture2D{})
	{
		type = SnailType::CHOPIN;
	}
	bool Start() override;
	bool CleanUp() override;
	void Update() override;
	void ApplySnailStats() override;
	float GetAbilityCooldown() override {
		return abilityTimer;
	}
private:
	void UseAbility();
private:
	float abilityTimer = 0.0f;
	float flyingDuration = 10.0f;
	bool isFlying = false;
	Texture2D flyTexture;
	Texture2D usingTexture;
};