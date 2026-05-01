#include "CollisionManager.h"

OBB CollisionManager::BuildCubeOBB(const ObjectTransform& t)
{
    OBB obb{};

    //world position of cube centre
    obb.Center = XMFLOAT3(t.PosX, t.PosY, t.PosZ);

    obb.Extents = XMFLOAT3(1.0f * t.Scaler, 1.0f * t.Scaler, 1.0f * t.Scaler); //change values if difference in space between vertices is not 1

    XMMATRIX rotation = XMMatrixRotationX(t.RotX) * XMMatrixRotationY(t.RotY) * XMMatrixRotationZ(t.RotZ);

    //locals of the box
    XMVECTOR localRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR localUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR localForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //rotate locals to be world space
    XMVECTOR worldRight = XMVector3TransformNormal(localRight, rotation);
    XMVECTOR worldUp = XMVector3TransformNormal(localUp, rotation);
    XMVECTOR worldForward = XMVector3TransformNormal(localForward, rotation);

    worldRight = XMVector3Normalize(worldRight);
    worldUp = XMVector3Normalize(worldUp);
    worldForward = XMVector3Normalize(worldForward);

    XMStoreFloat3(&obb.Axis[0], worldRight);
    XMStoreFloat3(&obb.Axis[1], worldUp);
    XMStoreFloat3(&obb.Axis[2], worldForward);

    return obb;
}

bool CollisionManager::CheckOBBOverlap(const OBB& a, const OBB& b)
{
    BoundingOrientedBox boxA;
    boxA.Center = a.Center;
    boxA.Extents = a.Extents;
    // Convert our 3 axes into rotation quaternion
    XMMATRIX rotA(
        a.Axis[0].x, a.Axis[0].y, a.Axis[0].z, 0.0f,
        a.Axis[1].x, a.Axis[1].y, a.Axis[1].z, 0.0f,
        a.Axis[2].x, a.Axis[2].y, a.Axis[2].z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    BoundingOrientedBox boxB;
    boxB.Center = b.Center;
    boxB.Extents = b.Extents;
    XMMATRIX rotB(
        b.Axis[0].x, b.Axis[0].y, b.Axis[0].z, 0.0f,
        b.Axis[1].x, b.Axis[1].y, b.Axis[1].z, 0.0f,
        b.Axis[2].x, b.Axis[2].y, b.Axis[2].z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    XMVECTOR qA = XMQuaternionRotationMatrix(rotA);
    XMVECTOR qB = XMQuaternionRotationMatrix(rotB);
    XMStoreFloat4(&boxA.Orientation, qA);
    XMStoreFloat4(&boxB.Orientation, qB);

    return boxA.Intersects(boxB);
}

void CollisionManager::LoopHallwayCheck(vector<Mesh> wrldmesh, int index, PlayerCamera& cam, ObjectTransform& playertransform, int& CorridorLoopCount)
{
    if (wrldmesh[index].TileIndex == 5) //if the colliding tile is of index 5
    {
        if (CorridorLoopCount < 4) //if loop count is less than 4
        {
            for (int i = 0; i < wrldmesh.size(); i++) //issue is that this loop doesnt stop when first tile of index 4 is found
            {
                if (wrldmesh[i].TileIndex == 4) //Teleport player to start of coridor
                {
                    cam.Position.x = wrldmesh[i].ObjTransform.PosX + 1;
                    cam.Position.y = wrldmesh[i].ObjTransform.PosY + 3;
                    cam.Position.z = wrldmesh[i].ObjTransform.PosZ;
                    cam.Yaw = 0;
                    playertransform.PosX = cam.Position.x;
                    playertransform.PosY = cam.Position.y; //matching camera default position
                    playertransform.PosZ = cam.Position.z;
                    CorridorLoopCount++;
                    OutputDebugString(L"what");
                    return;
                }
            }
            return;
        }
        else { wrldmesh[index].TileIndex = 4; }
    }
}

bool CollisionManager::MeshIndexCheck(int tileIndex, bool& CanMove)
{
    if (tileIndex == 1 ||
        tileIndex == 2 ||
        tileIndex == 3 ||
        tileIndex == 1 ||
        tileIndex == 6 ||
        tileIndex == 7 ||
        tileIndex == 8) {
        CanMove = false;//if colliding with any of these tiles, player should not move
    }
    return CanMove;
}

void CollisionManager::FallingTunnelCheck(vector<Mesh> wrldmesh, int tileindex, PlayerCamera& cam, ObjectTransform& playertransform)
{
    if (wrldmesh[tileindex].TileIndex == 7)
    {
        for (int a = 0; a < wrldmesh.size(); a++)
        {
            if (wrldmesh[a].TileIndex == 8) //Teleport player to stop of tunnel loop
            {
                playertransform.PosX = wrldmesh[a].ObjTransform.PosX + 0.5f;
                playertransform.PosY = wrldmesh[a].ObjTransform.PosY - 2.0f; //matching camera default position
                playertransform.PosZ = wrldmesh[a].ObjTransform.PosZ + 1.0f;
                cam.Position.x = playertransform.PosX;
                cam.Position.y = playertransform.PosY;
                cam.Position.z = playertransform.PosZ;
                return;
            }
        }
    }
}

