#define _CRT_SECURE_NO_WARNINGS
#include "SourceDlg.h"
using namespace std;

CSourceDlg* CSourceDlg::ptr = NULL;
CSourceDlg::CSourceDlg(void)
{
	ptr = this;
}
void CSourceDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}
void OpenConsole()
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
}
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	//OpenConsole();
	cout << "Console boot\n";
	CSourceDlg dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, CSourceDlg::DlgProc);
}

void CSourceDlg::IsWin(HWND hwnd)
{
	if (aGameNumbers.size() == aUserNumbers.size())
	{
		KillTimer(hwnd, pTimer);
		MessageBox(hwnd, TEXT("You win"), TEXT("You win"), 0);
		bGameStarted = FALSE;
	}
}
BOOL CSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	hSpin = GetDlgItem(hDialog, IDC_SPIN1);
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	hList = GetDlgItem(hDialog, IDC_LIST1);
	hStartGame = GetDlgItem(hDialog, IDC_STARTGAME);
	hProgBar = GetDlgItem(hDialog, IDC_PROGRESS1);

	SendMessage(hProgBar, PBM_SETPOS, 0, 0);
	SendMessage(hProgBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(hProgBar, PBM_SETSTEP, 1, 0);
	SendMessage(hProgBar, PBM_SETBKCOLOR, 0, LPARAM(RGB(224, 224, 224)));
	SendMessage(hProgBar, PBM_SETBARCOLOR, 0, LPARAM(RGB(0, 128, 128)));
	SendMessage(hSpin, UDM_SETRANGE32, 0, 120);
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);
	SetWindowText(hEdit, TEXT("0"));

	for (size_t i = 0; i < 16; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON1 + i);
		int num = IDC_BUTTON1 + i;
		hButtons.push_back({ hButton, num });
		EnableWindow(hButton, FALSE);
	}

	return TRUE;
}
int RandNum(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}
void CSourceDlg::GenerateNumbers()
{
	for (size_t i = 0; i < hButtons.size(); i++)
	{
		aGameNumbers.push_back(RandNum(1, 100));
		EnableWindow(hButtons[i].hwnd, TRUE);
		wsprintf(buf, TEXT("%d"), aGameNumbers[i]);
		SetWindowText(hButtons[i].hwnd, buf);
	}
}
void CSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_STARTGAME)
	{
		int nEditTime = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
		if (nEditTime <= 0)
		{
			MessageBox(hwnd, TEXT("Set value must be greater than 0"), TEXT("Error"), NULL);
			return;
		}
		nTimeMsec = nEditTime * 5;
		SendMessage(hProgBar, PBM_SETRANGE, 0, MAKELPARAM(0, nTimeMsec));
		SendMessage(hProgBar, PBM_SETPOS, 0, 0);
		SetTimer(hwnd, pTimer, 200, NULL);

		for (size_t i = 0; i < aUserNumbers.size(); i++)
		{
			SendMessage(hList, LB_DELETESTRING, 0, 0);
		}
		aGameNumbers.clear();
		aUserNumbers.clear();
		GenerateNumbers();
		sort(aGameNumbers.begin(), aGameNumbers.end());
		bGameStarted = TRUE;
	}

	if (bGameStarted)
	{

		for (size_t i = 0; i < hButtons.size(); i++)
		{
			if (id == hButtons[i].BtnId)
			{
				GetWindowText(hButtons[i].hwnd, buf, BUFSIZE);
				int num = _tstoi(buf);
				if (aGameNumbers[aUserNumbers.size()] == num)
				{
					aUserNumbers.push_back(num);
					EnableWindow(hButtons[i].hwnd, FALSE);
					SendMessage(hList, LB_ADDSTRING, 0, LPARAM(buf));
				}
				IsWin(hwnd);
			}
		}
	}
}
void CSourceDlg::Cls_OnTimer(HWND hwnd, UINT id)
{
	int nProgNum = SendMessage(hProgBar, PBM_GETPOS, 0, 0);
	if (nProgNum >= nTimeMsec)
	{
		KillTimer(hwnd, pTimer);
		MessageBox(hwnd, TEXT("Out of time"), TEXT("Timer stopped"), 0);
		bGameStarted = FALSE;
		return;
	}
	SendMessage(hProgBar, PBM_STEPIT, 0, 0);
}

INT_PTR CALLBACK CSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}