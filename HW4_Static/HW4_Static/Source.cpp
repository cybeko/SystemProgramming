#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <vector>
#include <tchar.h>
#include "resource.h"
#include "stdio.h"
#include <iostream>
using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

struct StaticStruct
{
	HWND hwnd;
	int downX;
	int downY;
	int width;
	int height;
};

vector<StaticStruct> hStatics;

TCHAR coordinates[100];
HINSTANCE hInst;

int userDownX, userDownY;
int userUpX, userUpY;
int width, height;

void OpenConsole()
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
}
void SwapNumbers(int* n1, int* n2)
{
	int temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	//OpenConsole();
	hInst = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;

	case WM_LBUTTONDBLCLK:
		userDownX = LOWORD(lParam);
		userDownY = HIWORD(lParam);

		for (int i = 0; i < hStatics.size(); ++i)
		{
			const StaticStruct& staticInfo = hStatics[i];
			if (userDownX >= staticInfo.downX && userDownX <= staticInfo.downX + staticInfo.width && userDownY
				>= staticInfo.downY && userDownY <= staticInfo.downY + staticInfo.height)
			{
				DestroyWindow(hStatics[i].hwnd);
				hStatics.erase(hStatics.begin() + i);
				break;
			}
		}

		return TRUE;

	case WM_COMMAND:

		return TRUE;

	case WM_RBUTTONDOWN:
		userDownX = LOWORD(lParam);
		userDownY = HIWORD(lParam);

		for (int i = hStatics.size() - 1; i >= 0; --i)
		{
			const StaticStruct& staticInfo = hStatics[i];
			if (userDownX >= staticInfo.downX && userDownX <= staticInfo.downX + staticInfo.width && userDownY >= staticInfo.downY && userDownY <= staticInfo.downY + staticInfo.height)
			{
				wsprintf(coordinates, TEXT("Static ID:%d; [X: %d][Y: %d]; Width: %d; Height: %d;"), i, staticInfo.downX, staticInfo.downY, staticInfo.width, staticInfo.height);
				SetWindowText(hWnd, coordinates);
				break;
			}
			else
			{
				SetWindowText(hWnd, L"");
			}
		}
		return TRUE;

	case WM_LBUTTONDOWN:
		userDownX = LOWORD(lParam);
		userDownY = HIWORD(lParam);

		return TRUE;

	case WM_LBUTTONUP:
		userUpX = LOWORD(lParam);
		userUpY = HIWORD(lParam);

		if (userUpX > userDownX)
		{
			width = userUpX - userDownX;
		}
		else
		{
			width = userDownX - userUpX;
			SwapNumbers(&userUpX, &userDownX);
		}
		if (userUpY > userDownY)
		{
			height = userUpY - userDownY;
		}
		else
		{
			height = userDownY - userUpY;
			SwapNumbers(&userUpY, &userDownY);
		}

		if (width > 10 && height > 10)
		{
			HWND hStatic = CreateWindowEx(0, TEXT("STATIC"), 0, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE, userDownX, userDownY, width, height, hWnd, 0, hInst, 0);
			hStatics.push_back({ hStatic, userDownX, userDownY, width, height });
		}
		else if ((width >= 2 && width < 10) || (height >= 2 && height < 10))
		{
			MessageBox(0, L"The static cannot be smaller than 10x10", TEXT("Incorrect size"), MB_OK | MB_ICONINFORMATION);
		}
		return TRUE;
	}
	return FALSE;
}