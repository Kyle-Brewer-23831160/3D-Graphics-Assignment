#pragma once
#include "Mesh.h"
#include "PlayerCamera.h"
#include <DirectXCollision.h>

using namespace DirectX;
using namespace std;
class CollisionManager
{
public:
	static OBB BuildCubeOBB(const ObjectTransform& t);
	static bool CheckOBBOverlap(const OBB& a, const OBB& b);
	static void LoopHallwayCheck(vector<Mesh> wrldmesh, int index, PlayerCamera& cam, ObjectTransform& playertransform, int& CorridorLoopCount);
	static bool MeshIndexCheck(int tileindex, bool& CanMove);
	static void FallingTunnelCheck(vector<Mesh> wrldmesh, int tileindex, PlayerCamera& cam, ObjectTransform& playertransform);
private:

};

