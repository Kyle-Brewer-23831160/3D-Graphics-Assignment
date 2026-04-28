#pragma once
#include <DirectXMath.h>
#include "Mesh.h"

class PlayerCamera
{
public:

    XMMATRIX GetCamView();
    void Move(float forward, float strafe, float deltaTime);
    float Yaw = 0.0f;   // Left/Right rotation
    float Pitch = 0.0f; // Up/Down rotation
    XMFLOAT3 Position = { 45.0f, -5.0f, 24.0f };
};

