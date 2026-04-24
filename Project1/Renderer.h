// Renderer.h
#pragma once

#include "DDSTextureLoader.h"
#include "Mesh.h"
#include "InputDetector.h"
#include "TileMap.h"
#include "WICTextureLoader.h"
#include <d3dcompiler.h>
#include <string>

using namespace Microsoft::WRL;
using namespace DirectX;

struct SkyboxConstantBuffer
{
    XMMATRIX vp;
};

struct SkyboxVertexData
{
    XMFLOAT3 position;
};

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
    void CreateViewMatrix();
    void CreateProjectionMatrix();
    void CreateWorldMatrix(float mAngle);
    void CreateConstantBuffer();
    void CreateTriangleGeometry();
    void CreateStencilBuffer();
    ID3D11ShaderResourceView* LoadTexture(const wchar_t* TextureAdress);
    void LoadCubeMap();
    void CreateSkyboxGeometry();

    // Per-frame bindings
    void SetPipelineState();
    void UpdateConstantBuffer(XMMATRIX OBJWorldMatrix);
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

    //skybox
    ComPtr<ID3D11ShaderResourceView> SkyboxTextureView;
    ComPtr<ID3D11VertexShader> mSkyboxVertexShader;
    ComPtr<ID3D11PixelShader>  mSkyboxPixelShader;
    ComPtr<ID3DBlob>           mSkyboxVertexShaderBlob;
    ComPtr<ID3DBlob>           mSkyboxPixelShaderBlob;
    ComPtr<ID3D11InputLayout>  mSkyboxInputLayout;
    ComPtr<ID3D11Buffer> mSkyboxConstantBuffer;
    ComPtr<ID3D11Buffer> SkyboxVertexBuffer;
    ComPtr<ID3D11Buffer> SkyboxIndexBuffer;

    //Texture
    ComPtr<ID3D11SamplerState> mSamplerState;
    ComPtr<ID3D11ShaderResourceView> mTextureView;


    float mPreviousTime;
    float mAngle;

    // Geometry
    TileMap TMmanager;
    std::vector<Mesh, std::allocator<Mesh>> WorldMesh;
    //Mesh CUBE1 = Mesh(0, 0, 0, WorldMesh);
    //Mesh CUBE2 = Mesh(3, 0, 0, WorldMesh);
    //Mesh CUBE3 = Mesh(0, 1, 0, WorldMesh);

    //Input Detection
    InputDetector detector = InputDetector(mHwnd);

    //Cmaera Matrices
    PlayerCamera mCam;
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
};
