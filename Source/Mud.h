#pragma once
#include "Globals.h"
#include "ModulePhysics.h"

#include <vector>

class Mud {
public:
	Mud()
	{
	}
	
	std::vector<b2Vec2> vertices;
	int newfriction = 2.5;
};