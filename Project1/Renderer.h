// Renderer.h
#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

using namespace Microsoft::WRL;
using namespace DirectX;

class Renderer
{
public:
    explicit Renderer(HWND hwnd);
    ~Renderer() = default;

    void RenderFrame(UINT mIndexCount);                 // Clear + bind pipeline + draw + present
    void ClearColor(XMFLOAT4 color);    // Optional if you want to call it manually

    ID3D11Device* GetDevice() const { return mDevice.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return mDeviceContext.Get(); }

private:
    // Init steps
    void CreateDevice();
    void CreateSwapChain();
    void CreateRenderTargetView();
    void CreateShaders();
    void CreateInputLayout();

    // Per-frame bindings
    void SetPipelineState();

public:
    HWND mHwnd = nullptr;

    ComPtr<ID3D11Device> mDevice;
    ComPtr<ID3D11DeviceContext> mDeviceContext;
    ComPtr<IDXGISwapChain> mSwapChain;
    ComPtr<ID3D11RenderTargetView> mRenderTargetView;

    // Shaders + Buffer layout
    ComPtr<ID3D11VertexShader> mVertexShader;
    ComPtr<ID3D11PixelShader>  mPixelShader;
    ComPtr<ID3DBlob>           mVertexShaderBlob;
    ComPtr<ID3DBlob>           mPixelShaderBlob;
    ComPtr<ID3D11InputLayout>  mInputLayout;

    // Geometry
    
};
