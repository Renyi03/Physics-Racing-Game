#pragma once
#include "Snail.h"

class MikuSnail : public Snail {
public:
	MikuSnail(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: Snail(physics, _x, _y, _listener, _texture)
	{
	}

	void Update() override;

	void UseAbility();

private:
};