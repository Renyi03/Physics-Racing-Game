#pragma once
#include "Box.h"
#include "Snail.h"
#include "Projectile.h"

class EnhypenSnail : public Snail {
public:
	EnhypenSnail(ModulePhysics* physics, int _x, int _y, Module* _listener)
		: Snail(physics, _x, _y, _listener, Texture{})
	{
		type = SnailType::ENHYPEN;
	}
	bool Start() override;
	bool CleanUp() override;
	void Update() override;
	void ApplySnailStats() override;
	void UseAbility();
	float GetAbilityCooldown() override {
		return abilityCooldown;
	}
private:
	float abilityCooldown = 0.0f;
	Projectile* projectile = nullptr;
};