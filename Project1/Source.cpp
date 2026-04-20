// main.cpp : Defines the entry point for the application.

#include "windows.h"
#include"Renderer.h"

using namespace Microsoft;

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

    Renderer* basicRenderer = new Renderer(g_hWnd);

    MSG msg = { 0 }; // Initialise the message structure
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        basicRenderer->RenderFrame();
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
