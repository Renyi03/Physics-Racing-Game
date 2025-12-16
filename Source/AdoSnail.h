#pragma once
#include "Box.h"
#include "Snail.h"

class AdoSnail : public Snail {
public:
	AdoSnail(ModulePhysics* physics, int _x, int _y, Module* _listener)
		: Snail(physics, _x, _y, _listener, Texture2D{})
	{
		type = SnailType::ADO;
	}
	bool Start() override;
	bool CleanUp() override;
	void Update() override;
	void ApplySnailStats() override;
	void UseAbility();
};