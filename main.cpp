#include <windows.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        // Handle window destruction
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        // Handle window painting
        ValidateRect(hWnd, NULL);
        break;

    default:
        // Forward unhandled messages to the default window procedure
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
   

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPWSTR lpCmdLine,
        _In_ int nCmdShow)
    {
        const wchar_t CLASS_NAME[] = L"MyWindowClass";

        WNDCLASSEX wcex = { 0 };
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszClassName = CLASS_NAME;


        if (!RegisterClassEx(&wcex))
        {
            MessageBox(NULL, L"Window registration failed!", L"Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        // Step 2: Create the window
        HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"Pauls Project",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            500, 300, NULL, NULL, hInstance, NULL);

        if (!hWnd)
        {
            MessageBox(NULL, L"Window creation failed!", L"Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        // Step 3: Show and update the window
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        // Step 4: Message loop
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
        

//Figuring out pull requests
    }