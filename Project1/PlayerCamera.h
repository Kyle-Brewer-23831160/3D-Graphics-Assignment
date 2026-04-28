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
    XMFLOAT3 Position = { 50.0f, -20.0f, 50.0f };
};

