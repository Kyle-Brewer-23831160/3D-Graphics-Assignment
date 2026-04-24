#include "CollisionManager.h"

OBB CollisionManager::BuildCubeOBB(const ObjectTransform& t)
{
    OBB obb{};

    //world position of cube centre
    obb.Center = XMFLOAT3(t.PosX, t.PosY, t.PosZ);

    obb.Extents = XMFLOAT3(1.0f * t.Scaler, 1.0f * t.Scaler, 1.0f * t.Scaler); //change values if difference in space between vertices is not 1

    XMMATRIX rotation = XMMatrixRotationZ(t.RotZ) * XMMatrixRotationZ(t.RotX) * XMMatrixRotationZ(t.RotY);

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
