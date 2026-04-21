#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")




#include <dinput.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Mesh.h"

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
    void DetectInput(ObjectTransform& t);
};

