#include "InputDetector.h"
#include <d3d11.h>

InputDetector::InputDetector(HWND hwnd)
{
    mHwnd = hwnd;
    InitialiseDirectInput();
    InitialiseKeyboardDevice();
    InitialiseMouseDevice();
}

void InputDetector::InitialiseDirectInput()
{
    HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, &mDI, nullptr);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to initialise DirectInput");
        return;
    }
}

void InputDetector::InitialiseKeyboardDevice()
{
    //Initialise the Keyboard device
        HRESULT hr = mDI->CreateDevice(GUID_SysKeyboard, &mKeyboardDevice, nullptr);
    if (FAILED(hr)) OutputDebugString(L"Failed to create Keyboard Device!");


    hr = mKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);  //Set Keyboard format
    if (FAILED(hr)) OutputDebugString(L"Failed to set Keyboard data format!");


    hr = mKeyboardDevice->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) OutputDebugString(L"Failed to set cooperative level for Keyboard!");


    mKeyboardDevice->Acquire(); //Acquire the device to start receiving input.
}

void InputDetector::InitialiseMouseDevice()
{
    // Initialise the mouse device
    HRESULT hr = mDI->CreateDevice(GUID_SysMouse, &mMouseDevice, nullptr);
    if (FAILED(hr)) { OutputDebugString(L"Failed to create Mouse Device!"); }

    hr = mMouseDevice->SetDataFormat(&c_dfDIMouse); //Set Keyboard format
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set Mouse data format!");
    }

    hr = mMouseDevice->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to set cooperative level for Mouse!");
    }

    mMouseDevice->Acquire(); //Acquire the device to start receiving input.
}

void InputDetector::DetectInput(ObjectTransform& t)
{
    currentTime = GetTickCount64() / 1000.0f;
    float deltaTime = currentTime - previousTime;
    previousTime = currentTime;


    // -------------------------
    // Keyboard
    // -------------------------
    if (mKeyboardDevice)
    {
        BYTE keyboardState[256];
        HRESULT hr = mKeyboardDevice->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
        if (FAILED(hr))
        {
            OutputDebugString(L"Failed to get keyboard state!\n");
            mKeyboardDevice->Acquire();
        }
        else
        {
            const float Speed = 1.5f;


            // Rotate around Y: A / D
            if (keyboardState[DIK_A] & 0x80) t.RotY += Speed * deltaTime;
            if (keyboardState[DIK_D] & 0x80) t.RotY -= Speed * deltaTime;


            // Rotate around Z: W / S
            if (keyboardState[DIK_W] & 0x80) t.RotX += Speed * deltaTime;
            if (keyboardState[DIK_S] & 0x80) t.RotX -= Speed * deltaTime;


            // Translate: Arrow keys
            if (keyboardState[DIK_LEFT] & 0x80) t.PosX -= Speed * deltaTime;
            if (keyboardState[DIK_RIGHT] & 0x80) t.PosX += Speed * deltaTime;


            if (keyboardState[DIK_UP] & 0x80) t.PosY += Speed * deltaTime;
            if (keyboardState[DIK_DOWN] & 0x80) t.PosY -= Speed * deltaTime;
        }
    }


    // -------------------------
    // Mouse
    // -------------------------
    if (mMouseDevice)
    {
        DIMOUSESTATE mouseState;
        HRESULT hr = mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
        if (FAILED(hr))
        {
            OutputDebugString(L"Failed to get mouse state!\n");
            mMouseDevice->Acquire();
        }
        else
        {
            // Wheel delta: positive = scroll up, negative = scroll down
            const float scaleStep = 0.10f;


            if (mouseState.lZ > 0) t.Scaler += scaleStep;
            if (mouseState.lZ < 0) t.Scaler -= scaleStep;


            // Clamp to avoid negative/zero scale
            t.Scaler = t.Scaler > 0.1f ? t.Scaler : 0.1f;

        }
    }
}
