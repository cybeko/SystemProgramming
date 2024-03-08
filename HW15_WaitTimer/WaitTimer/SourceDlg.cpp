#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER
#include "SourceDlg.h"
#include "framework.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

CWaitableTimerDlg* CWaitableTimerDlg::ptr = NULL;

struct ThreadParams 
{
	int index;
	HWND hListState;
};
ThreadParams* paramList = new ThreadParams;

CWaitableTimerDlg::CWaitableTimerDlg(void)
{
	ptr = this;
	pNID = new NOTIFYICONDATA;
}

CWaitableTimerDlg::~CWaitableTimerDlg(void)
{
	delete pNID;
}

void CWaitableTimerDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

UINT editIds[] = { IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4 };

BOOL CWaitableTimerDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hEditHours = GetDlgItem(hwnd, IDC_EDIT1);
	hEditMinutes = GetDlgItem(hwnd, IDC_EDIT2);
	hEditSeconds = GetDlgItem(hwnd, IDC_EDIT3);

	hSpin1 = GetDlgItem(hwnd, IDC_SPIN1);
	hSpin2 = GetDlgItem(hwnd, IDC_SPIN2);
	hSpin3 = GetDlgItem(hwnd, IDC_SPIN3);

	hListTaskNames = GetDlgItem(hwnd, IDC_LIST1);
	hListTime = GetDlgItem(hwnd, IDC_LIST2);

	hListStatus = GetDlgItem(hwnd, IDC_LIST3);

	hBAddTask = GetDlgItem(hwnd, IDC_BUTTON1);
	hBDeleteTask = GetDlgItem(hwnd, IDC_BUTTON3);
	hBFinishTask = GetDlgItem(hwnd, IDC_BUTTON4);

	hEditTaskName = GetDlgItem(hwnd, IDC_EDIT4);

	hDialog = hwnd;

	SendMessage(hSpin1, UDM_SETRANGE32, 0, 23);
	SendMessage(hSpin2, UDM_SETRANGE32, 0, 59);
	SendMessage(hSpin3, UDM_SETRANGE32, 0, 59);

	HINSTANCE hInst = GetModuleHandle(NULL);

	memset(pNID, 0, sizeof(NOTIFYICONDATA));
	pNID->cbSize = sizeof(NOTIFYICONDATA);
	pNID->hIcon = hIcon;
	pNID->hWnd = hwnd;
	lstrcpy(pNID->szTip, TEXT("Timer"));

	pNID->uCallbackMessage = WM_ICON;

	pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;

	lstrcpy(pNID->szInfo, TEXT("Timer"));
	lstrcpy(pNID->szInfoTitle, TEXT("Timer"));
	pNID->uID = ID_TRAYICON;
	return TRUE;
}

DWORD WINAPI Thread(LPVOID lp)
{
	CWaitableTimerDlg* p = (CWaitableTimerDlg*)lp;
	int length = SendMessage(p->hEditTaskName, WM_GETTEXTLENGTH, 0, 0);
	TCHAR* pBuffer = new TCHAR[length + 1];
	TCHAR state[] = TEXT("Current");

	GetWindowText(p->hEditTaskName, pBuffer, length + 1);
	SetWindowText(p->hEditTaskName, TEXT(""));

	SendMessage(p->hListTaskNames, LB_ADDSTRING, 0, LPARAM(pBuffer));
	SendMessage(p->hListStatus, LB_ADDSTRING, 0, LPARAM(state));

	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	TCHAR buf[10];
	int hours, minutes, seconds;
	GetWindowText(p->hEditHours, buf, 10);
	hours = _tstoi(buf);
	GetWindowText(p->hEditMinutes, buf, 10);
	minutes = _tstoi(buf);
	GetWindowText(p->hEditSeconds, buf, 10);
	seconds = _tstoi(buf);

	SendMessage(p->hSpin1, UDM_SETPOS32, 0, LPARAM(0));
	SendMessage(p->hSpin2, UDM_SETPOS32, 0, LPARAM(0));
	SendMessage(p->hSpin3, UDM_SETPOS32, 0, LPARAM(0));

	TCHAR time[100];
	wsprintf(time, TEXT("%d:%d:%d"), hours, minutes, seconds);
	SendMessage(p->hListTime, LB_ADDSTRING, 0, LPARAM(time));
	SYSTEMTIME st;

	GetLocalTime(&st);
	if (st.wHour > hours || st.wHour == hours && st.wMinute > minutes ||
		st.wHour == hours && st.wMinute == minutes && st.wSecond > seconds)
	{
		return 0;
	}
	else if (st.wHour < hours && st.wMinute < minutes && st.wSecond < seconds)
	{
		MessageBox(p->hDialog, TEXT("Incorrect time"), TEXT("Add task"), MB_OK | MB_ICONSTOP);

	}
	st.wHour = hours;
	st.wMinute = minutes;
	st.wSecond = seconds;

	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft, &ft);
	SetWaitableTimer(hTimer, (LARGE_INTEGER*)&ft, 0, NULL, NULL, FALSE);

	if (WaitForSingleObject(hTimer, INFINITE) == WAIT_OBJECT_0) {
		MessageBox(0, _T("Time is up."), 0, 0);
		int index = SendMessage(p->hListStatus, LB_GETCURSEL, 0, 0);
		SendMessage(p->hListStatus, LB_DELETESTRING, index, 0);
		SendMessage(p->hListStatus, LB_INSERTSTRING, index, LPARAM(_T("Overdue")));
	}
	return 0;
}


void CWaitableTimerDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	int check = 0;

	if (id == IDC_BUTTON1)
	{
		int length = SendMessage(hEditTaskName, WM_GETTEXTLENGTH, 0, 0);
		TCHAR* pBuffer = new TCHAR[length + 1];

		if (lstrlen(pBuffer))
		{
			int index = SendMessage(hListTaskNames, LB_FINDSTRINGEXACT, -1, LPARAM(pBuffer));
			if (index == LB_ERR)
			{
				for (UINT i = 0; i < sizeof(editIds) / sizeof(editIds[0]); ++i) 
				{
					HWND hEdit = GetDlgItem(hwnd, editIds[i]);

					int textLength = GetWindowTextLength(hEdit);

					if (textLength <= 0) 
					{
						check++;
					}
				}

				if (check == 0)
				{
					HANDLE h;
					h = CreateThread(NULL, 0, Thread, this, 0, NULL);
				}
				else
				{
					MessageBox(hwnd, TEXT("Fields must not be empty"), TEXT("Add task"), MB_OK | MB_ICONSTOP);
				}

			}
			else
			{
				MessageBox(hwnd, TEXT("Task with this name already exists"), TEXT("Add task"), MB_OK | MB_ICONSTOP);
			}

		}
		delete[] pBuffer;
	}

	else if (id == IDC_BUTTON3) 
	{
		int index = SendMessage(hListTaskNames, LB_GETCURSEL, 0, 0);
		SendMessage(hListTaskNames, LB_DELETESTRING, index, 0);
		SendMessage(hListTime, LB_DELETESTRING, index, 0);
		SendMessage(hListStatus, LB_DELETESTRING, index, 0);
	}

	else if (id == IDC_BUTTON4)
	{
		int index = SendMessage(hListTaskNames, LB_GETCURSEL, 0, 0);

		if (index != LB_ERR)
		{
			TCHAR state[] = TEXT("Done");
			SendMessage(hListStatus, LB_DELETESTRING, index, 0);
			SendMessage(hListStatus, LB_INSERTSTRING, index, LPARAM(state));
		}
		else
		{
			MessageBox(hwnd, TEXT("Select a task from hListTasks to mark as 'Done'"), TEXT("Error"), MB_OK | MB_ICONSTOP);
		}
	}
}

BOOL CALLBACK CWaitableTimerDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);

	}
	return FALSE;
}