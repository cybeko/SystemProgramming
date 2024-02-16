#include <Windows.h>
#include "resource.h"

HINSTANCE hInstance;

struct Static
{
    int FirX, FirY, SecX, SecY;
};

Static TopL, TopC, TopR;
Static BottomL, BottomC, BottomR;
Static CenterL, CenterR;

RECT WinRect, StaticRect;
HWND hStatic, hwnd;
const int StaticW = 70;
const int StaticH = 70;
int StaticX, StaticY;
int StaticXMove1, StaticXMove2, StaticYMove1, StaticYMove2;
const int StaticMargin = 25;
const int Speed = 6;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void MoveStatic(int x, int y)
{
    int newX = StaticX + x;
    int newY = StaticY + y;

    if (newX >= 0 && newX + StaticW <= WinRect.right - WinRect.left && newY >= 0 && newY + StaticH <= WinRect.bottom - WinRect.top)
    {
        MoveWindow(hStatic, newX, newY, StaticW, StaticH, TRUE);
        StaticX = newX;
        StaticY = newY;
    }
    else
    {
        SetWindowText(hStatic, L"Caught");
    }

}
void UpdateStaticRect()
{
    GetWindowRect(hStatic, &StaticRect);
    StaticX = StaticRect.left - 10;
    StaticY = StaticRect.top - 34;

    StaticXMove1 = StaticX - StaticMargin;
    StaticXMove2 = StaticX + StaticW + StaticMargin;
    StaticYMove1 = StaticY - StaticMargin;
    StaticYMove2 = StaticY + StaticH + StaticMargin;

    TopL = { StaticX - StaticMargin, StaticY - StaticMargin, StaticX, StaticY };
    TopC = { StaticX, StaticY - StaticMargin, StaticX + StaticW, StaticY };
    TopR = { StaticX + StaticW, StaticY - StaticMargin, StaticX + StaticW + StaticMargin, StaticY };
    CenterL = { StaticX - StaticMargin, StaticY, StaticX, StaticY + StaticH };
    CenterR = { StaticX + StaticW, StaticY, StaticX + StaticW + StaticMargin, StaticY + StaticH };
    BottomL = { StaticX - StaticMargin, StaticY + StaticH, StaticX - StaticMargin, StaticY + StaticH + StaticMargin };
    BottomC = { StaticX, StaticY + StaticH, StaticX + StaticW, StaticY + StaticH + StaticMargin };
    BottomR = { StaticX + StaticW, StaticY + StaticH, StaticX + StaticW + StaticMargin, StaticY + StaticH + StaticMargin };
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    ::hInstance = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int MouseX, MouseY;

    GetWindowRect(hwnd, &WinRect);
    const int WinWidth = WinRect.right - WinRect.left;
    const int WinHeight = WinRect.bottom - WinRect.top;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Static"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
            WinWidth / 2 - StaticW, WinHeight / 2 - StaticH, StaticW, StaticH, hwnd, 0, hInstance, 0);
        break;

    case WM_MOUSEMOVE:
        UpdateStaticRect();
        MouseX = LOWORD(lParam);
        MouseY = HIWORD(lParam);

        if (MouseX >= TopL.FirX && MouseX <= TopL.SecX && MouseY >= TopL.FirY && MouseY <= TopL.SecY)
        {
            MoveStatic(Speed, Speed);
        }
        else if (MouseX >= TopC.FirX && MouseX <= TopC.SecX && MouseY >= TopC.FirY && MouseY <= TopC.SecY)
        {
            MoveStatic(0, Speed);
        }
        else if (MouseX >= TopR.FirX && MouseX <= TopR.SecX && MouseY >= TopR.FirY && MouseY <= TopR.SecY)
        {
            MoveStatic(-Speed, Speed);
        }
        else if (MouseX >= BottomL.FirX && MouseX <= BottomL.SecX && MouseY >= BottomL.FirY && MouseY <= BottomL.SecY)
        {
            MoveStatic(Speed, -Speed);
        }
        else if (MouseX >= BottomC.FirX && MouseX <= BottomC.SecX && MouseY >= BottomC.FirY && MouseY <= BottomC.SecY)
        {
            MoveStatic(0, -Speed);
        }
        else if (MouseX >= BottomR.FirX && MouseX <= BottomR.SecX && MouseY >= BottomR.FirY && MouseY <= BottomR.SecY)
        {
            MoveStatic(-Speed, -Speed);
        }
        else if (MouseX >= CenterL.FirX && MouseX <= CenterL.SecX && MouseY >= CenterL.FirY && MouseY <= CenterL.SecY)
        {
            MoveStatic(Speed, 0);
        }
        else if (MouseX >= CenterR.FirX && MouseX <= CenterR.SecX && MouseY >= CenterR.FirY && MouseY <= CenterR.SecY)
        {
            MoveStatic(-Speed, 0);
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}