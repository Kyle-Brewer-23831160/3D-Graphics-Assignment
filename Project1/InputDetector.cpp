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

void InputDetector::DetectInput(PlayerCamera& cam, HWND hWnd, int ScreenSizeX, int ScreenSizeY, float &forward, float &side)
{
    currentTime = GetTickCount64() / 1000.0f;
    float deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    forward = 0;
    side = 0;

    POINT pt;
    pt.x = ScreenSizeX / 2;
    pt.y = ScreenSizeY/ 2;

    ClientToScreen(hWnd, &pt);
    if(!Pause)  SetCursorPos(pt.x, pt.y);

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
            if (keyboardState[DIK_A] & 0x80)  side = -1.0f;
            if (keyboardState[DIK_D] & 0x80) side = 1.0f;

            if (keyboardState[DIK_W] & 0x80) forward = 1.0f;
            if (keyboardState[DIK_S] & 0x80) forward = -1.0f;

            if (keyboardState[DIK_ESCAPE] & 0x80)
            {
                if (!ESCHELD)
                {
                    Pause = !Pause;
                    ESCHELD = true;
                }
            }
            else
            {
                ESCHELD = false;
            }
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
            if (mouseState.lX > 0) cam.Yaw += 0.02f;
            if (mouseState.lX < 0) cam.Yaw -= 0.02f;

            if (cam.Pitch < 2.0f)  if(mouseState.lY > 0) cam.Pitch += 0.02f;
            if (cam.Pitch > -2.0f)  if(mouseState.lY < 0) cam.Pitch -= 0.02f;
        }
    }
}
