#include <windows.h>
#include "Resource.h"

INT_PTR CALLBACK BananaDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap = NULL;
   
    switch (message)
    {
    case WM_INITDIALOG:
    {
        if (hBitmap == NULL)
        {
            hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BANANA_IMAGE), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
        }
        SendDlgItemMessage(hDlg, IDC_BANANA_PICTURE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_CLOSE:
    {
        if (hBitmap)
        {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
        EndDialog(hDlg, 0);
        return TRUE;
    }
    }


    return FALSE;
}

INT_PTR CALLBACK LOWRESPICDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap = NULL;
    const int padding = 40;
    const int bottompadding = 40;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        if (hBitmap == NULL)
        {
            hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOWRESPIC_IMAGE), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
        }
        SendDlgItemMessage(hDlg, IDC_LOWRESPIC_PICTURE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

        // Get the image dimensions
        BITMAP bitmapInfo;
        GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);
        int imageWidth = bitmapInfo.bmWidth;
        int imageHeight = bitmapInfo.bmHeight;

        // Get screen dimensions
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        float aspectRatio = (float)imageWidth / (float)imageHeight;
        if (imageWidth > screenWidth)
        {
            imageWidth = screenWidth;
            imageHeight = (int)(screenWidth / aspectRatio);
        }
        if (imageHeight > screenHeight)
        {
            imageHeight = screenHeight;
            imageWidth = (int)(screenHeight * aspectRatio);
        }

        // Resize the dialog window
        SetWindowPos(hDlg, NULL, 0, 0, imageWidth + padding * 2, imageHeight + bottompadding * 2, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

        // Center the dialog window on the screen
        int x = (screenWidth - imageWidth - padding * 2) / 2;
        int y = (screenHeight - imageHeight - bottompadding * 2) / 2;
        SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);

        // Move the image control
        HWND hImageControl = GetDlgItem(hDlg, IDC_LOWRESPIC_PICTURE);
        x = padding;
        y = padding;
        SetWindowPos(hImageControl, NULL, x, y, imageWidth, imageHeight, SWP_NOZORDER | SWP_NOACTIVATE);

        return TRUE;
    }
    case WM_CLOSE:
    {
        if (hBitmap)
        {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
        EndDialog(hDlg, 0);
        return TRUE;
    }
    case WM_SIZE:
    {
        int newWidth = LOWORD(lParam);
        int newHeight = HIWORD(lParam);

        HWND hImageControl = GetDlgItem(hDlg, IDC_LOWRESPIC_PICTURE);

        int imageWidth = 0, imageHeight = 0;

        if (hBitmap != NULL)
        {
            BITMAP bitmapInfo;
            GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);
            imageWidth = bitmapInfo.bmWidth;
            imageHeight = bitmapInfo.bmHeight;
        }
        // Update imageWidth and imageHeight based on new dimensions of dialog window
        float aspectRatio = (float)imageWidth / (float)imageHeight;
        imageWidth = newWidth;
        imageHeight = (int)(newWidth / aspectRatio);
        if (imageHeight > newHeight)
        {
            imageHeight = newHeight;
            imageWidth = (int)(newHeight * aspectRatio);
        }

        int newX = (newWidth - imageWidth) / 2;
        int newY = (newHeight - imageHeight) / 2;

        // Resize and reposition the image control
        SetWindowPos(hImageControl, NULL, newX, newY, imageWidth, imageHeight, SWP_NOZORDER | SWP_NOACTIVATE);

        // Update the image displayed by the image control
        SendDlgItemMessage(hDlg, IDC_LOWRESPIC_PICTURE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

        return TRUE;
    }

    }


    return FALSE;

}








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
    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case ID_FILE_EXIT:
            // Handle "Exit" command
            PostQuitMessage(0);
            break;

        case ID_HELP_ABOUT:
            // Handle "About" command
            MessageBox(hWnd, L"Paul's Project\nVersion 1.2", L"About", MB_OK | MB_ICONINFORMATION);
            break;

        case ID_SOUND_ROAR:
            PlaySound(MAKEINTRESOURCE(IDS_ROAR_SOUND), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
            return TRUE;

        case ID_MENU_BANANA:
            // Handle "Banana" command
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_BANANA_DIALOG), hWnd, BananaDialogProc);
            break;
        case ID_MENU_LOWRESPIC:
            // Handle "LOWRESPIC" command
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_LOWRESPIC_DIALOG), hWnd, LOWRESPICDialogProc);
            break;
        
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }

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
            500, 300, NULL, LoadMenu(hInstance, MAKEINTRESOURCE(MENU_MAIN)), hInstance, NULL);

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