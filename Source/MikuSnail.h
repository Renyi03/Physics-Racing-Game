#pragma once
#include "Box.h"
#include "Snail.h"

class MikuSnail : public Snail {
public:
	MikuSnail(ModulePhysics* physics, int _x, int _y, Module* _listener)
		: Snail(physics, _x, _y, _listener, Texture{})
	{
		type = SnailType::MIKU;
	}
	bool Start() override;
	bool CleanUp() override;
	void Update() override;
	void ApplySnailStats() override;

	void UseAbility();
private:
	float abilityTimer = 0.0f;

};