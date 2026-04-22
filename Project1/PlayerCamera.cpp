#include "PlayerCamera.h"

using namespace DirectX;

XMMATRIX PlayerCamera::GetCamView()
{
    // Create rotation matrix from Euler angles
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw(Pitch, Yaw, 0.0f);

    // Calculate where the camera is looking
    XMVECTOR lookAt = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rotation);
    XMVECTOR up = XMVector3TransformCoord(XMVectorSet(0, 1, 0, 0), rotation);

    XMVECTOR posVec = XMLoadFloat3(&Position);
    return XMMatrixLookAtLH(posVec, posVec + lookAt, up);
}


void PlayerCamera::Move(float forward, float strafe, float deltaTime)
{
    float speed = 10.0f * deltaTime;

    // Move relative to where we are facing
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0, Yaw, 0);
    XMVECTOR forwardVec = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rotation);
    XMVECTOR rightVec = XMVector3TransformCoord(XMVectorSet(1, 0, 0, 0), rotation);

    XMVECTOR posVec = XMLoadFloat3(&Position);
    posVec += forwardVec * forward * speed;
    posVec += rightVec * strafe * speed;
    XMStoreFloat3(&Position, posVec);
}
