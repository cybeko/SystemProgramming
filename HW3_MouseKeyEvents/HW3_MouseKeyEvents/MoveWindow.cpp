#include "Windows.h"
#include "tchar.h"
#include <string>
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
        L"Press Enter",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, //x,y
        CW_USEDEFAULT, CW_USEDEFAULT, //width, height
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
bool isEnter = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_RETURN)
        {
            MoveWindow(hwnd, 0, 0, 300, 300, true);
            SetWindowText(hwnd, L"Use arrows to move the window");
            isEnter = true;
        }
        if (isEnter)
        {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int size = 300;

            switch (wParam)
            {
            case VK_LEFT:
                MoveWindow(hwnd, rect.left - 15, rect.top, size, size, true);
                break;

            case VK_RIGHT:
                MoveWindow(hwnd, rect.left + 15, rect.top, size, size, true);
                break;

            case VK_UP:
                MoveWindow(hwnd, rect.left, rect.top - 15, size, size, true);
                break;

            case VK_DOWN:
                MoveWindow(hwnd, rect.left, rect.top + 15, size, size, true);
                break;
            }
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
