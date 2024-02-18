#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
#include <iostream>
#include <vector>
using namespace std;

struct Button
{
    HWND hwnd;
    int buttonId;
    bool isCrossOrNull[2] = {FALSE, FALSE};
};

HINSTANCE hAppInstance;
HBITMAP hBitmapCross;
HBITMAP hBitmapNull;
bool bCrossTurn;
TCHAR buf[150];
static HWND hRadioUser, hRadioComp;
static bool bGameStarted = false;
vector<Button> hButtons;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitButtons(HWND hwnd)
{
    for (size_t i = 0; i < 9; i++)
    {
        HWND hButton = GetDlgItem(hwnd, IDC_BUTTON1 + i);
        int num = IDC_BUTTON1 + i;
        hButtons.push_back({ hButton, num});
        SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
        cout << "\nbutton init" << i;
    }
}
void OpenConsole()
{
    AllocConsole();
    (void)freopen("conin$", "r", stdin);
    (void)freopen("conout$", "w", stdout);
    (void)freopen("conout$", "w", stderr);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    //OpenConsole();
    cout << "console output";
    hAppInstance = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

void NewGame(HWND hwnd)
{
    for (size_t i = 0; i < hButtons.size(); i++)
    {
        SendDlgItemMessage(hwnd, hButtons[i].buttonId, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
        EnableWindow(hButtons[i].hwnd, TRUE);
    }
    hButtons.clear();
    InitButtons(hwnd);
    bCrossTurn = true;
    bGameStarted = false;
    CheckDlgButton(hwnd, IDC_RADIO_USER, BST_UNCHECKED);
    SetWindowText(hwnd, L"Press 'New game'");
    CheckDlgButton(hwnd, IDC_RADIO_COMP, BST_UNCHECKED);
}
void PrintVector(const vector<Button>& buttons)
{
    cout << endl<<"Print vector:\n";
    for (const auto& button : buttons)
    {
        cout << "Button ID: " << button.buttonId << ", isCrossOrNull: [" << button.isCrossOrNull[0] << ", " << button.isCrossOrNull[1] <<"]"<<endl;
    }
    cout << endl << "End of vector\n";
}
bool CheckWin(HWND hwnd)
{
    int winCombinations[8][3] = 
    {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++)
    {
        int a = winCombinations[i][0];
        int b = winCombinations[i][1];
        int c = winCombinations[i][2];

        cout << endl<<"checking combination: [" << a << "][" << b << "][" << c<<"]";
        PrintVector(hButtons);

        if (hButtons[a].isCrossOrNull[0] && hButtons[b].isCrossOrNull[0] && hButtons[c].isCrossOrNull[0])
        {
            MessageBox(hwnd, L"X user wins", L"Game Over", MB_OK);
            NewGame(hwnd);
            return true;
        }
        else if (hButtons[a].isCrossOrNull[1] && hButtons[b].isCrossOrNull[1] && hButtons[c].isCrossOrNull[1])
        {
            MessageBox(hwnd, L"O comp wins", L"Game Over", MB_OK);
            NewGame(hwnd);
            return true;
        }
        else
        {
            cout << endl << "[Match not found]";
        }
    }

    bool isTie = true;

    for (size_t i = 0; i < hButtons.size(); i++)
    {
        if (!hButtons[i].isCrossOrNull[0] && !hButtons[i].isCrossOrNull[1])
        {
            isTie = false;
            break;
        }
    }
    if (isTie)
    {
        MessageBox(hwnd, L"It's a tie", L"Game Over", MB_OK);
        NewGame(hwnd);
        return true;
    }
    return false;
}
void ComputerTurn(HWND hwnd)
{
    vector<int> enabledButtons;

    for (size_t i = 0; i < hButtons.size(); i++)
    {
        if (IsWindowEnabled(hButtons[i].hwnd))
        {
            enabledButtons.push_back(i);
        }
    }

    if (!enabledButtons.empty())
    {
        int randId = rand() % enabledButtons.size();
        int selectedIndex = enabledButtons[randId];

        SendDlgItemMessage(hwnd, hButtons[selectedIndex].buttonId, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapNull);
        hButtons[selectedIndex].isCrossOrNull[1] = true;
        SetWindowText(hwnd, L"User turn");
        EnableWindow(hButtons[selectedIndex].hwnd, FALSE);

        if (CheckWin(hwnd))
        {
            return;
        }
    }
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        hBitmapCross = (HBITMAP)LoadImage(hAppInstance, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
        hBitmapNull = (HBITMAP)LoadImage(hAppInstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
        SetWindowText(hwnd, L"Press 'New game'");
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON_NEW_GAME && HIWORD(wParam) == BN_CLICKED)
        {
            NewGame(hwnd);
            SetWindowText(hwnd, L"Select who goes first");

        }
        else if (!bGameStarted) 
        {
            if (IsDlgButtonChecked(hwnd, IDC_RADIO_USER) == BST_CHECKED)
            {
                bCrossTurn = true;
            }
            else if (IsDlgButtonChecked(hwnd, IDC_RADIO_COMP) == BST_CHECKED)
            {
                bCrossTurn = false;
                ComputerTurn(hwnd);
                bCrossTurn = true;

            }
            else
            {
                MessageBox(hwnd, L"Please select who goes first", L"Error", MB_OK | MB_ICONERROR);
                return TRUE;
            }
            bGameStarted = true;
        }
        else
        {
            for (size_t i = 0; i < hButtons.size(); i++)
            {
                if (LOWORD(wParam) == hButtons[i].buttonId && HIWORD(wParam) == BN_CLICKED)
                {
                    if (bCrossTurn)
                    {
                        SetWindowText(hwnd, L"User turn");
                        SendDlgItemMessage(hwnd, hButtons[i].buttonId, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapCross);
                        hButtons[i].isCrossOrNull[0] = true;
                        CheckWin(hwnd);
                        EnableWindow(hButtons[i].hwnd, FALSE);
                        ComputerTurn(hwnd);
                    }

                }
            }
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}
