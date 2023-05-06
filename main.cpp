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

    switch (message)
    {
    case WM_INITDIALOG:
    {
        if (hBitmap == NULL)
        {
            hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOWRESPIC_IMAGE), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
        }
        SendDlgItemMessage(hDlg, IDC_LOWRESPIC_PICTURE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_SIZE:
    {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        // Resize the image control
        HWND hImageControl = GetDlgItem(hDlg, IDC_LOWRESPIC_PICTURE);
        SetWindowPos(hImageControl, NULL, (width - 230) / 2, 10, width - 20, height - 20, SWP_NOZORDER | SWP_NOACTIVATE);

        // Load the original image
        HBITMAP hOriginalBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOWRESPIC_IMAGE), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

        // Create a new resized image maintaining aspect ratio
        HBITMAP hResizedBitmap = NULL;
        {
            HDC hdcScreen = GetDC(NULL);
            HDC hdcOriginal = CreateCompatibleDC(hdcScreen);
            HDC hdcResized = CreateCompatibleDC(hdcScreen);

            HBITMAP hOldOriginalBitmap = (HBITMAP)SelectObject(hdcOriginal, hOriginalBitmap);

            // Calculate new width and height while maintaining aspect ratio
            BITMAP originalBitmapInfo;
            GetObject(hOriginalBitmap, sizeof(BITMAP), &originalBitmapInfo);
            int originalWidth = originalBitmapInfo.bmWidth;
            int originalHeight = originalBitmapInfo.bmHeight;
            float aspectRatio = (float)originalWidth / originalHeight;

            int newWidth = (int)((height - 20) * aspectRatio);
            int newHeight = height - 20;

            // Create a compatible bitmap with the new dimensions
            hResizedBitmap = CreateCompatibleBitmap(hdcScreen, newWidth, newHeight);
            HBITMAP hOldResizedBitmap = (HBITMAP)SelectObject(hdcResized, hResizedBitmap);

            // Stretch the original image to fit the new dimensions
            SetStretchBltMode(hdcResized, HALFTONE);
            StretchBlt(hdcResized, 0, 0, newWidth, newHeight, hdcOriginal, 0, 0, originalWidth, originalHeight, SRCCOPY);

            SelectObject(hdcOriginal, hOldOriginalBitmap);
            SelectObject(hdcResized, hOldResizedBitmap);

            DeleteDC(hdcOriginal);
            DeleteDC(hdcResized);
            ReleaseDC(NULL, hdcScreen);
        }

        // Set the resized image to the image control
        SendMessage(hImageControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hResizedBitmap);

        // Clean up
        DeleteObject(hOriginalBitmap);
        if (hBitmap)
        {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
        hBitmap = hResizedBitmap;

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
            MessageBox(hWnd, L"Paul's Project\nVersion 1.0", L"About", MB_OK | MB_ICONINFORMATION);
            break;

        /*case ID_MENU_BANANA:
            // Handle "Banana" command
            MessageBox(hWnd, L"You clicked the Banana Item!", L"Banana", MB_OK | MB_ICONINFORMATION);
            break;*/
        case ID_MENU_BANANA:
            // Handle "Banana" command
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_BANANA_DIALOG), hWnd, BananaDialogProc);
            break;
        case ID_MENU_LOWRESPIC:
            // Handle "Hentai" command
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