// Renderer.h
#pragma once

#include "Mesh.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "InputDetector.h"
#include "WICTextureLoader.h"
#include <d3dcompiler.h>
#include <string>

using namespace Microsoft::WRL;
using namespace DirectX;


struct ConstantBuffer
{
    XMMATRIX WVP;
};

struct VertexData
{
    XMFLOAT3 position;
    XMFLOAT2 texCoord;
};

class Renderer
{
public:
    explicit Renderer(HWND hwnd);
    ~Renderer() = default;

    void RenderFrame();                 // Clear + bind pipeline + draw + present
    void RenderStartScreenUI(HWND mHWnd);
    void ClearColor(XMFLOAT4 color);    // Optional if you want to call it manually

    ID3D11Device* GetDevice() const { return mDevice.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return mDeviceContext.Get(); }

private:
    // Init steps
    void CompileTileMaps();
    void CreateDevice();
    void CreateSwapChain();
    void CreateRenderTargetView();
    void CreateShaders();
    void CreateInputLayout();
    void CreateProjectionMatrix();
    void CreateWorldMatrix(float mAngle);
    void CreateConstantBuffer();
    void CreateTriangleGeometry();
    void CreateStencilBuffer();
    ID3D11ShaderResourceView* LoadTexture(const wchar_t* TextureAdress);

    // Per-frame bindings
    void SetPipelineState();
    void UpdateConstantBuffer(XMMATRIX OBJWorldMatrix, XMMATRIX camMat);
    void BindGeometry();

public:
    HWND mHwnd = nullptr;
    RECT rect;

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
    ComPtr<ID3D11Buffer> mConstantBuffer;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> IndexBuffer;
    ComPtr<ID3D11Texture2D> mdepthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> mdepthStencilView;

    //Texture
    ComPtr<ID3D11SamplerState> mSamplerState;
    ComPtr<ID3D11ShaderResourceView> mTextureView;


    float mPreviousTime;
    float mAngle;

    // Geometry
    TileMap TMmanager;
    std::vector<Mesh, std::allocator<Mesh>> WorldMesh;

    Mesh PlayerBox;

    //Input Detection
    InputDetector detector = InputDetector(mHwnd);

    //Camera Matrices
    PlayerCamera mCam;
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
    float forward;
    float side;

    //game states
    int state = 0;
    bool looptriggered;
    int coridoorLoopCount;
};
