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
    XMFLOAT3 Position = { 0.0f, 0.0f, -10.0f };
};

