#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")



#include "Mesh.h"
#include "PlayerCamera.h"
#include <dinput.h>
#include <d3d11.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

class InputDetector
{
public:
    InputDetector(HWND hwnd);
    ~InputDetector() = default;
    HWND mHwnd = nullptr;
    float currentTime = 0.0f;
    float previousTime = 0.0f;
    float yAngle = 0.0f;
    ComPtr<IDirectInput8> mDI;
    ComPtr<IDirectInputDevice8> mKeyboardDevice;
    ComPtr<IDirectInputDevice8> mMouseDevice;

    void InitialiseDirectInput();
    void InitialiseKeyboardDevice();
    void InitialiseMouseDevice();
    void DetectInput(PlayerCamera& cam, HWND hWnd, int ScreenSizeX, int ScreenSizeY, float& forward, float& side);

private:
    bool Pause = false;
};


