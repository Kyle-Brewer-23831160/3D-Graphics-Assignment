// main.cpp : Defines the entry point for the application.

#include "windows.h"
#include"Renderer.h"

using namespace Microsoft;
struct VertexData
{
    XMFLOAT3 position;
    XMFLOAT4 color;
};

VertexData vertices[] =
{
    { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1, 0, 0, 1) }, // Red
    { XMFLOAT3(0.0f,  0.5f, 0.0f), XMFLOAT4(0, 1, 0, 1) }, // Green
    { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0, 0, 1, 1) }  // Blue
};


uint32_t indices[] =
{
    0, 1, 2
};

// Global variables
HINSTANCE g_hInst;	// current instance
HWND g_hWnd;		// main window handle



// Function declarations (included in this C++ file)
ATOM				MyRegisterClass(HINSTANCE);
BOOL				CreateMainWnd(int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


// Main entry point function
int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    /* UNREFERENCED_PARAMETER(hPrevInstance);
     UNREFERENCED_PARAMETER(lpCmdLine);*/

     // Initialisation
    g_hInst = hInstance;
    g_hWnd = NULL;

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"BlankWndClass";
    wcex.hIconSm = NULL;

    RegisterClassEx(&wcex);


    // Create main window
    if (!CreateMainWnd(nCmdShow))
        return -1;

    //++++++++++++++++++D3D11+++++++++++++++++++++++++++++ 
    Renderer* basicRenderer = new Renderer(g_hWnd);

    ComPtr<ID3D11Buffer> mVertexBuffer;
    ComPtr<ID3D11Buffer> mIndexBuffer;
    UINT  mIndexCount = sizeof(indices) / sizeof(indices[0]);

    D3D11_BUFFER_DESC vbDesc{};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = vertices;

    HRESULT hr = basicRenderer->GetDevice()->CreateBuffer(&vbDesc, &vbData, mVertexBuffer.GetAddressOf());
    if (FAILED(hr))
        OutputDebugString(L"FAILED: CreateBuffer vertex\n");
    else
        OutputDebugString(L"SUCCESS: CreateBuffer vertex\n");

    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(int) * mIndexCount;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = indices;

    hr = basicRenderer->GetDevice()->CreateBuffer(&ibDesc, &ibData, mIndexBuffer.GetAddressOf());
    if (FAILED(hr))
        OutputDebugString(L"FAILED: CreateBuffer index\n");
    else
        OutputDebugString(L"SUCCESS: CreateBuffer index\n");

    UINT stride = sizeof(VertexData);
    UINT offset = 0;

    ID3D11Buffer* vb = mVertexBuffer.Get();
    basicRenderer->mDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
    basicRenderer->mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

    MSG msg = { 0 }; // Initialise the message structure
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        basicRenderer->RenderFrame(mIndexCount);
    }

    return (int)msg.wParam; // Return the exit code
}


BOOL CreateMainWnd(int nCmdShow) {
    // Calculate window size
    RECT rc = { 0, 0, 800, 600 }; // Desired client area size 
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE); // Adjust for window style

    // Create the window
    g_hWnd = CreateWindow(
        L"BlankWndClass",          // Registered class name
        L"Blank Window",           // Window title
        WS_OVERLAPPEDWINDOW,       // Window style
        CW_USEDEFAULT,             // X position (default)
        CW_USEDEFAULT,             // Y position (default)
        rc.right - rc.left,        // Width (adjusted)
        rc.bottom - rc.top,        // Height (adjusted)
        NULL,                      // Parent window (none)
        NULL,                      // Menu (none)
        g_hInst,                   // Application instance
        NULL                       // Additional data (none)
    );

    // Check if window creation succeeded
    if (g_hWnd == NULL)
        return FALSE;

    // Show and update the window
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}


// Processes messages for the main window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
