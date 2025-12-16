#pragma once
#include "Globals.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

#include <vector>

class Ice {
public:
	Ice()
	{
	}

	ColliderType type = ColliderType::ICE;
	PhysicCategory category = PhysicCategory::MAP;

	static constexpr int vertices[58] = {
		1191, 1132,
		1206, 1132,
		1217, 1136,
		1230, 1148,
		1238, 1168,
		1254, 1202,
		1291, 1235,
		1316, 1261,
		1332, 1281,
		1342, 1304,
		1347, 1325,
		1353, 1339,
		1353, 1360,
		1332, 1404,
		1313, 1422,
		1300, 1428,
		1284, 1432,
		1227, 1432,
		1191, 1423,
		1155, 1405,
		1134, 1387,
		1119, 1374,
		1102, 1340,
		1095, 1306,
		1095, 1249,
		1106, 1203,
		1120, 1178,
		1144, 1154,
		1168, 1139
	};
};