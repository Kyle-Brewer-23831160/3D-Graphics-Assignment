// Renderer.cpp
#include "Renderer.h"
#include <windows.h>

Renderer::Renderer(HWND hwnd) : mHwnd(hwnd)
{
    CreateDevice();
    CreateSwapChain();
    CreateRenderTargetView();
    CreateShaders();
    CreateInputLayout();
    CreateTriangleGeometry();
    CreateStencilBuffer();

    CreateProjectionMatrix();
    CreateViewMatrix();
    CreateConstantBuffer();

    CompileTileMaps();
}

void Renderer::CompileTileMaps()
{
    ID3D11ShaderResourceView* whiteTex = LoadTexture(L"Textures\\White.jpg");
    ID3D11ShaderResourceView* blackTex = LoadTexture(L"Textures\\Black.jpg");
    ID3D11ShaderResourceView* greenTex = LoadTexture(L"Textures\\Green.jpg");

    PlayerBox = Mesh(0, 0, 0, whiteTex);

    int rows = sizeof(TMmanager.TileMap1Layout) / sizeof(TMmanager.TileMap1Layout[0]);
    int Columns = sizeof(TMmanager.TileMap1Layout[0]) / sizeof(TMmanager.TileMap1Layout[0][0]);

    for (int a = 0; a < TMmanager.TileMaps.size(); a++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < Columns; j++)
            {
                if (TMmanager.TileMaps[a].TileMap[i][j] == 1) //Tilemap "a" at row "i" and column "j" //1 = default cube
                {
                    Mesh NewCube = Mesh(j, a, 1 - i, whiteTex);
                    NewCube.TileIndex = 1;
                    WorldMesh.push_back(NewCube);
                }
                else if (TMmanager.TileMaps[a].TileMap[i][j] == 2)
                {
                    Mesh NewCube = Mesh(j, a, 1 - i, blackTex);
                    NewCube.TileIndex = 2;
                    WorldMesh.push_back(NewCube);
                }
                else if (TMmanager.TileMaps[a].TileMap[i][j] == 3)
                {
                    Mesh NewCube = Mesh(j, a, 1 - i, greenTex);
                    NewCube.TileIndex = 3;
                    WorldMesh.push_back(NewCube);
                }
            }
        }
    }

    PlayerBox.ObjTransform.PosX = 10;
    PlayerBox.ObjTransform.PosY = -1.5; //matching camera default position
    PlayerBox.ObjTransform.PosZ = -10;
}

void Renderer::CreateTriangleGeometry()
{
    VertexData vertices[] =
 {
   // Front Face
   { {-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f} },
   { {-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f} },
   { { 1.0f, 1.0f, -1.0f}, {1.0f, 0.0f} },
   { { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f} },
 
   // Back Face
   { {-1.0f, -1.0f, 1.0f}, {1.0f, 1.0f} },
   { { 1.0f, -1.0f, 1.0f}, {0.0f, 1.0f} },
   { { 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
   { {-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
 
   // Top Face
   { {-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f} },
   { {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
   { { 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
   { { 1.0f, 1.0f, -1.0f}, {1.0f, 1.0f} },
 
   // Bottom Face
   { {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f} },
   { { 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f} },
   { { 1.0f, -1.0f, 1.0f}, {0.0f, 0.0f} },
   { {-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f} },
 
   // Left Face
   { {-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f} },
   { {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
   { {-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f} },
   { {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f} },
 
   // Right Face
   { { 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f} },
   { { 1.0f, 1.0f, -1.0f}, {0.0f, 0.0f} },
   { { 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
   { { 1.0f, -1.0f, 1.0f}, {1.0f, 1.0f} },
 };
 
 
 uint32_t indices[] = {
   // Front Face
   0, 1, 2,
   0, 2, 3,
 
   // Back Face
   4, 5, 6,
   4, 6, 7,
 
   // Top Face
   8, 9, 10,
   8, 10, 11,
 
   // Bottom Face
   12, 13, 14,
   12, 14, 15,
 
   // Left Face
   16, 17, 18,
   16, 18, 19,
 
   // Right Face
   20, 21, 22,
   20, 22, 23
 };

    //VERTEX BUFFER & DATA DESCRIPTION
    D3D11_BUFFER_DESC vbDesc{};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = vertices;

    HRESULT Result = mDevice->CreateBuffer(&vbDesc, &vbData, vertexBuffer.GetAddressOf());

    if (FAILED(Result)) (L"FAILED TO CREATE VERTEX BUFFER");

    //INDEX BUFFER & DATA DESCRIPTION
    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(int) * 36;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = indices;

    Result = mDevice->CreateBuffer(&ibDesc, &ibData, IndexBuffer.GetAddressOf());

    if (FAILED(Result)) (L"FAILED TO CREATE INDEX BUFFER");
}

void Renderer::CreateDevice()
{
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels,
        1,
        D3D11_SDK_VERSION,
        mDevice.GetAddressOf(),
        nullptr,
        mDeviceContext.GetAddressOf()
    );

    if (FAILED(hr))
        OutputDebugString(L"FAILED: D3D11CreateDevice\n");
    else
        OutputDebugString(L"SUCCESS: D3D11CreateDevice\n");
}

void Renderer::CreateSwapChain()
{
    GetClientRect(mHwnd, &rect);

    DXGI_SWAP_CHAIN_DESC scd{};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = rect.right - rect.left;
    scd.BufferDesc.Height = rect.bottom - rect.top;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = mHwnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<IDXGIDevice> dxgiDevice;
    HRESULT hr = mDevice.As(&dxgiDevice);
    if (FAILED(hr))
    {
        OutputDebugString(L"FAILED: Query IDXGIDevice\n");
        return;
    }

    ComPtr<IDXGIAdapter> dxgiAdapter;
    hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugString(L"FAILED: GetAdapter\n");
        return;
    }

    ComPtr<IDXGIFactory> dxgiFactory;
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        OutputDebugString(L"FAILED: GetParent IDXGIFactory\n");
        return;
    }

    hr = dxgiFactory->CreateSwapChain(mDevice.Get(), &scd, mSwapChain.GetAddressOf());
    if (FAILED(hr))
        OutputDebugString(L"FAILED: CreateSwapChain\n");
    else
        OutputDebugString(L"SUCCESS: CreateSwapChain\n");
}

void Renderer::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr))
    {
        OutputDebugString(L"FAILED: GetBuffer(backBuffer)\n");
        return;
    }

    hr = mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf());
    if (FAILED(hr))
        OutputDebugString(L"FAILED: CreateRenderTargetView\n");
    else
        OutputDebugString(L"SUCCESS: CreateRenderTargetView\n");
}

void Renderer::CreateShaders()
{
    ComPtr<ID3DBlob> errorBlob;
    const std::string pixelShaderCode  = R"(

    struct PS_INPUT
    {
        float4 position : SV_POSITION;
        float2 inTexCoord    : TEXCOORD;
    };

    Texture2D texObject : register(t0);
    SamplerState objSampleState : register(s0);

    float4 main(PS_INPUT input) : SV_TARGET
    {
        float3 pixelcolour = texObject.Sample(objSampleState, input.inTexCoord);
        return float4(pixelcolour, 1.0f);
    }
    )";

    D3DCompile(pixelShaderCode.c_str(), pixelShaderCode.length(), nullptr, nullptr, nullptr, "main", "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &mVertexShaderBlob, &errorBlob);

    if (errorBlob.Get() != nullptr && errorBlob->GetBufferPointer() != nullptr)
        OutputDebugString((LPCWSTR)errorBlob->GetBufferPointer());
    if (FAILED(mDevice->CreatePixelShader(mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf())))
    {
        OutputDebugString(L"Failed to craete Pixel Shader!\n");
        abort();
    }
    else {
        OutputDebugString(L"Successful to craete Pixel Shader!\n");

    }



    ComPtr<ID3DBlob> errorBlob1;
    const std::string vertexShaderCode = R"(

                    cbuffer ConstantBuffer : register(b0)
                    {
                        matrix WVP;
                    }

                    struct vertexIn
                    {
                        float3 position : POSITION;
                        float2 inTexCoord    : TEXCOORD;
                    };

                    struct vertexOut
                    {
                       float4 position : SV_POSITION;
                       float2 outTexCoord : TEXCOORD;
                    };

                    vertexOut main(vertexIn input)
                    {
                        vertexOut output;
                        output.position = mul(float4(input.position, 1.0f), WVP);
                        output.outTexCoord = input.inTexCoord;      
                        return output;
                    }
                )";


    D3DCompile(vertexShaderCode.c_str(), vertexShaderCode.length(), nullptr, nullptr, nullptr, "main", "vs_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &mVertexShaderBlob, &errorBlob1);

    if (errorBlob1.Get() != nullptr && errorBlob1->GetBufferPointer() != nullptr)
        OutputDebugString((LPCWSTR)errorBlob1->GetBufferPointer());

    if (FAILED(mDevice->CreateVertexShader(mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf())))
    {
        OutputDebugString(L"Failed to craete Vertex Shader!\n");
        abort();
    }
    else {
        OutputDebugString(L"Successful to craete Vertex Shader!\n");

    }
}

void Renderer::CreateInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    mDevice->CreateInputLayout(inputElementDesc, 2, mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), mInputLayout.GetAddressOf());
}

void Renderer::CreateViewMatrix()
{
    mView = XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f), //Cam is 10 steps back
        XMVectorSet(0.0f, 0.0f, 50.0f, 1.0f), 
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
        );
}

void Renderer::CreateProjectionMatrix()
{
    float width = float(rect.right - rect.left);
    float height = float(rect.bottom - rect.top);

    mProjection = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(45.0f), //FOV in angle
        width / height, //Aspect Ratio
        0.1f, //Near plane
        1000.0f //Far plane
    );
}

void Renderer::CreateWorldMatrix(float mAngle)
{
    XMMATRIX translation = XMMatrixTranslation(0, 0, 0);
    XMMATRIX ROTY = XMMatrixRotationY(mAngle); // Rotation around Z
    //XMMATRIX ROTY = XMMatrixRotationZ(mAngle); // Rotation around Y
    XMMATRIX Scale = XMMatrixScaling(1, 1, 1); //Scaling factor (1,1,1) is no scaling

    mWorld = translation/* * ROTZ*/ * ROTY * Scale; //replace M world Render object when rendering multiple
}

void Renderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC cbd{};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT Result = mDevice->CreateBuffer(&cbd, nullptr, mConstantBuffer.GetAddressOf());
    
    if (FAILED(Result)) OutputDebugString (L"FAILED TO CREATE CONSTANT BUFFER\n"); 
}

void Renderer::UpdateConstantBuffer(XMMATRIX OBJWorldMatrix)
{
    //----Matrices----
    XMMATRIX camView = mCam.GetCamView();

    //if anything is wrong after movement do * mView
    XMMATRIX wvp = OBJWorldMatrix * camView * mProjection;

    //----Update GPU----
    ConstantBuffer CB;
    CB.WVP = XMMatrixTranspose(wvp);

    mDeviceContext->UpdateSubresource(mConstantBuffer.Get(),
        0,
        nullptr,
        &CB,
        0,
        0
    );
}

void Renderer::CreateStencilBuffer()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = 800;
    depthStencilDesc.Height = 600;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    mDevice->CreateTexture2D(&depthStencilDesc, NULL, &mdepthStencilBuffer);
    mDevice->CreateDepthStencilView(mdepthStencilBuffer.Get(), NULL, &mdepthStencilView);
}

ID3D11ShaderResourceView* Renderer::LoadTexture(const wchar_t *TextureAdress)
{
    //Create Texture Sampler
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    HRESULT hr = mDevice->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf());
    if (FAILED(hr)) OutputDebugString(L"Failed to create sampler state.");

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tempView;

    //Create Texture View
    hr = DirectX::CreateWICTextureFromFile(
        mDevice.Get(),                   // ID3D11Device*
        mDeviceContext.Get(),            // ID3D11DeviceContext*
        TextureAdress,       // const wchar_t*
        nullptr,
        tempView.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed Load Texture!\n");
        return nullptr;
    }
    OutputDebugString(L"Successfully Loaded Texture!\n");
    return tempView.Detach();
}

void Renderer::BindGeometry()
{
    UINT stride = sizeof(VertexData);
    UINT offset = 0;

    mDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    mDeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Renderer::SetPipelineState()
{
    mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mdepthStencilView.Get());
    mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

    mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
    mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
    mDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
    mDeviceContext->IASetInputLayout(mInputLayout.Get());

    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = TRUE;
    rasterDesc.DepthBias = 1000;
    rasterDesc.ScissorEnable = FALSE;
    rasterDesc.MultisampleEnable = FALSE;
    rasterDesc.AntialiasedLineEnable = FALSE;

    ComPtr<ID3D11RasterizerState> rasterState;
    mDevice->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf());

    mDeviceContext->RSSetState(rasterState.Get());
    GetClientRect(mHwnd, &rect);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(rect.right - rect.left);
    viewport.Height = static_cast<float>(rect.bottom - rect.top);
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    mDeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::ClearColor(XMFLOAT4 color)
{
    float clearColor[] = { color.x, color.y, color.z, color.w };
    mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), clearColor);
}

void Renderer::RenderFrame()
{
    //----TIME----
    float CurrentTime = GetTickCount64() / 1000.0f;//obtain the elapsed system time and convert into seconds
    float deltaTime = CurrentTime - mPreviousTime;
    mPreviousTime = CurrentTime;

    ClearColor({ 0.2f, 0.5f, 0.4f, 1.0f });

    mDeviceContext->ClearDepthStencilView(mdepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);

    //----CHECK FOR VALID MOVEMENTS----
    detector.DetectInput(mCam, mHwnd, 800, 600, forward, side); //get inputs

    ObjectTransform transformSave = PlayerBox.ObjTransform; //create transform save
    PlayerBox.Move(forward, side, deltaTime, mCam.Yaw);//move player transform to new position
    OBB movingOBB = CollisionManager::BuildCubeOBB(PlayerBox.ObjTransform); //build orient bounding box at that position

    bool CanMove = true;
    
    for (int i = 0; i < WorldMesh.size(); i++)
    {
       OBB fixedOBB = CollisionManager::BuildCubeOBB(WorldMesh[i].ObjTransform);

       bool isColliding = CollisionManager::CheckOBBOverlap(movingOBB, fixedOBB); //check bounding box collision against all other world objects

       if (isColliding) 
       { 
           if(WorldMesh[i].TileIndex == 3)
           { 
               WorldMesh.erase(WorldMesh.begin() + i);
           }
           else { CanMove = false; }
       } //if colliding with any, player should not move
    }

    //-----MOVE PLAYER-----
    if(CanMove) { mCam.Move(forward, side, deltaTime); } //if can move, update playercam and leave playerbox transform at new position
    else { PlayerBox.ObjTransform = transformSave; } //if cant move, revert changes to player box transform.


    //---RENDER WORLD----
    SetPipelineState();
    BindGeometry();

    for (int i = 0; i < WorldMesh.size(); i++)
    {
        WorldMesh[i].CreateWorldMatrix(WorldMesh[i].ObjTransform);
        UpdateConstantBuffer(WorldMesh[i].ReturnMatrix());
        ID3D11ShaderResourceView* texture = WorldMesh[i].GetTexture();
        mDeviceContext->PSSetShaderResources(0, 1, &texture);
        mDeviceContext->DrawIndexed(36, 0, 0); // 36 indices for one cube
    }

    mSwapChain->Present(1, 0);
}
