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
    RECT rect{};
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
    const std::string pixelShaderCode = R"(

                 float4 main() : SV_TARGET
                  {
                    return float4(1.0, 0.0, 0.0, 1.0);
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
                    struct vertexIn
                    {
                        float3 position : POSITION;
                        float4 color    : COLOR;
                    };

                    struct vertexOut
                    {
                       float4 position : SV_POSITION;
                       float4 color    : COLOR;
                    };

                    vertexOut main(vertexIn input)
                    {
                        vertexOut output;
                        output.position = float4(input.position, 1.0);
                        output.color = input.color;      
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
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    mDevice->CreateInputLayout(inputElementDesc, 2, mVertexShaderBlob->GetBufferPointer(), mVertexShaderBlob->GetBufferSize(), mInputLayout.GetAddressOf());

}


void Renderer::SetPipelineState()
{
    mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
    mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
    mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
    mDeviceContext->IASetInputLayout(mInputLayout.Get());

    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;  
    rasterDesc.DepthClipEnable = TRUE;

    ComPtr<ID3D11RasterizerState> rasterState;
    mDevice->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf());

    mDeviceContext->RSSetState(rasterState.Get());
    RECT rect;
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

void Renderer::RenderFrame(UINT mIndexCount)
{
    ClearColor({ 0.2f, 0.3f, 0.4f, 1.0f });
    SetPipelineState();
    mDeviceContext->DrawIndexed(3, 0, 0);
    mSwapChain->Present(1, 0);
}
