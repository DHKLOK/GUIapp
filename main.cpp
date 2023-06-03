#include <windows.h>
#include "Resource.h"
#pragma comment(lib, "winmm.lib")
#include <Richedit.h>
RECT rect;
int wmId;

INT_PTR CALLBACK WarningDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_YES:
			EndDialog(hDlg, ID_YES);
			return TRUE;
		case ID_NO:
			EndDialog(hDlg, ID_NO);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	}
	return FALSE;
}



//////////////////////////////////////////   B   A   N   A   N   A  ///////////////////////////////////////////////////////


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


/////////////////////////////////////////////////////// B     E      A     R     ///////////////////////////////////////////////////////////////////////


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

		HWND hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Play Roar"),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10, 10, 100, 30,
			hDlg, (HMENU)IDC_PLAY_ROAR, GetModuleHandle(NULL), NULL);

		if (hButton == NULL) {
			MessageBox(NULL, L"Button Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}

		return TRUE;
	}
	case WM_COMMAND:

	{
		switch (LOWORD(wParam))
		{
		case IDC_PLAY_ROAR:
		{
			// Store the result of the dialog box
			INT_PTR result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_WARNING_DIALOG), hDlg, WarningDialogProc);

			if (result == ID_YES)
			{
				// "Face the bear" was selected, play the sound
				PlaySound(MAKEINTRESOURCE(IDS_ROAR_SOUND), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
			}
			// No need to do anything when "Run away" was selected
			break;
		}
		}
		break;

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

//////////////////////////////////// WINDOW HANDLING //////////////////////////////////////////////////


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hStatic = NULL;
	static HBRUSH hbrBkgnd = NULL;


	switch (message) {

	case WM_DESTROY:
		if (hbrBkgnd)
			DeleteObject(hbrBkgnd);  // delete the brush
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);
		FillRect(hdc, &rect, hbrBkgnd);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_CREATE:
	{
		// Create the static text control.
		hStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Hello, Rainbow Text!"),
			WS_CHILD | WS_VISIBLE | SS_CENTER | WS_CLIPSIBLINGS,
			10, 10, 200, 100, hWnd, NULL,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

		if (hStatic == NULL)
		{
			MessageBox(NULL, TEXT("Failed to create static text!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
			return -1; // Fail WM_CREATE
		}

		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);  
		return (INT_PTR)hbrBkgnd;  
	}
	break;

	case WM_ERASEBKGND:
	{
		if (hbrBkgnd == NULL)
			hbrBkgnd = CreateSolidBrush(RGB(255, 0, 255));

		if (hbrBkgnd)
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			// Only fill the background if the color has changed
			if (wParam != NULL)
			{
				FillRect((HDC)wParam, &rect, hbrBkgnd);
				// Bring the static text control to the top
				if (hStatic != NULL)
					BringWindowToTop(hStatic);
			}

			return 1;
		}
		break;
	}
	case WM_COMMAND:
	{
		wmId = LOWORD(wParam);
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

		case ID_MENU_BANANA:
			// Handle "Banana" command
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_BANANA_DIALOG), hWnd, BananaDialogProc);
			break;
		case ID_MENU_LOWRESPIC:
			// Handle "LOWRESPIC" command
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_LOWRESPIC_DIALOG), hWnd, LOWRESPICDialogProc);
			break;
		case ID_BACKGROUND_RED:
			if (hbrBkgnd)
				DeleteObject(hbrBkgnd);
			hbrBkgnd = CreateSolidBrush(RGB(255, 0, 0));
			InvalidateRect(hWnd, NULL, TRUE);

			// Redraw the static text control
			if (hStatic != NULL)
				InvalidateRect(hStatic, NULL, TRUE);
			break;

		case ID_BACKGROUND_GREEN:
			if (hbrBkgnd)
				DeleteObject(hbrBkgnd);
			hbrBkgnd = CreateSolidBrush(RGB(0, 255, 0));
			InvalidateRect(hWnd, NULL, TRUE);

			// Redraw the static text control
			if (hStatic != NULL)
				InvalidateRect(hStatic, NULL, TRUE);
			break;

		case ID_BACKGROUND_BLUE:
			if (hbrBkgnd)
				DeleteObject(hbrBkgnd);
			hbrBkgnd = CreateSolidBrush(RGB(0, 0, 255));
			InvalidateRect(hWnd, NULL, TRUE);

			// Redraw the static text control
			if (hStatic != NULL)
				InvalidateRect(hStatic, NULL, TRUE);
			break;
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
	//LoadLibrary("Msftedit.dll");
{
	const wchar_t CLASS_NAME[] = L"MyWindowClass";

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszClassName = CLASS_NAME;


	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, L"Window registration failed!", L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	// Step 2: Create the window
	HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"Pauls Project",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600, NULL, LoadMenu(hInstance, MAKEINTRESOURCE(MENU_MAIN)), hInstance, NULL);

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
}