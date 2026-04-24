#pragma once
#include "Mesh.h"
#include <DirectXCollision.h>
using namespace DirectX;

class CollisionManager
{
public:
	static OBB BuildCubeOBB(const ObjectTransform& t);
	static bool CheckOBBOverlap(const OBB& a, const OBB& b);
};

