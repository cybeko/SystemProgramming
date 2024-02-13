#include <Windows.h>
#include "resource.h"
using namespace std;

HINSTANCE hInstance;
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    hInstance = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hStatic;

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        hStatic = GetDlgItem(hwnd, IDC_STATIC_INFO);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            int num1 = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, FALSE);
            int num2 = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, FALSE);

            int result = 0;

            if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED)
            {
                result = num1 + num2;
            }
            else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED)
            {
                result = num1 - num2;
            }
            else if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_CHECKED)
            {
                result = num1 * num2;
            }
            else
            {
                if (num2 != 0)
                {
                    result = num1 / num2;
                }
                else
                {
                    MessageBox(hwnd, TEXT("Division by 0"), TEXT("Error"), MB_OK | MB_ICONERROR);
                }
            }

            TCHAR buf[256];
            wsprintf(buf, TEXT("%d"), result);
            SetWindowText(hStatic, buf);
        }
        break;
        case IDCANCEL:
            EndDialog(hwnd, 0);
            return TRUE;
        }
    }
    break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}
