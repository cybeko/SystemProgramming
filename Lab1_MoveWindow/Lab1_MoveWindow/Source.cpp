#include "Windows.h"
#include "tchar.h"
#include <string>
#include <time.h>

using std::wstring;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WCHAR className[] = L"FirstWindow";

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = NULL;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;

	wndClass.lpfnWndProc = WindowProc;

	if (!RegisterClassEx(&wndClass)) {
		return 0;
	}

	HWND hWnd = CreateWindowEx(
		0,
		className,
		L"My first window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	SetWindowText(hWnd, L"Press Enter to move the window");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

VOID CALLBACK MoveWindow(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int nSize = 300;

	MoveWindow(hwnd, rect.left + 15, rect.top, nSize, nSize, true);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);

	if (rect.left + nSize >= screenWidth)
	{
		KillTimer(hwnd, 1);
		SetWindowText(hwnd, L"Press Enter");
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static time_t t;
	static TCHAR str[100];

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		t = time(NULL);	
		lstrcpy(str, _tctime(&t));
		str[lstrlen(str) - 1] = '\0';
		SetWindowText(hWnd, str);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			MoveWindow(hWnd, 0, 0, 300, 300, true);
			SetWindowText(hWnd, L"Press Esc");
			SetTimer(hWnd, 1, 100, MoveWindow);
		}
		else if (wParam == VK_ESCAPE)
		{
			KillTimer(hWnd, 1);
			SetWindowText(hWnd, L"Press Enter");
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}