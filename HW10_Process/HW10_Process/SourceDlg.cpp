#include "SourceDlg.h"

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
void CSourceDlg::UpdateSnapshot()
{
	RefreshSnapShot();

	if (Process32First(hSnapShot, &pe32))
	{
		for (size_t i = 0; i < myProcess.size(); i++)
		{
			SendMessage(hList, LB_DELETESTRING, 0, 0);
		}
		myProcess.clear();

		int id = SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
		SendMessage(hList, LB_SETITEMDATA, id, myProcess.size());
		myProcess.push_back(pe32);

		while (Process32Next(hSnapShot, &pe32))
		{
			int id = SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
			SendMessage(hList, LB_SETITEMDATA, id, myProcess.size());
			myProcess.push_back(pe32);
		}
	}
}
void CSourceDlg::RefreshSnapShot()
{
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	memset(&pe32, 0, sizeof(PROCESSENTRY32));
	pe32.dwSize = sizeof(PROCESSENTRY32);
}

BOOL CSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	hList = GetDlgItem(hDialog, IDC_LIST1);
	hButTerminate = GetDlgItem(hDialog, IDC_TERMINATE_PROC);
	hButRefresh = GetDlgItem(hDialog, IDC_REFRESH);
	hButInfo = GetDlgItem(hDialog, IDC_INFO);
	hButCreate = GetDlgItem(hDialog, IDC_NEWPROC);
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	UpdateSnapshot();
	return TRUE;
}

void CSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_REFRESH)
	{
		UpdateSnapshot();
	}
	else if (id == IDC_INFO)
	{
		int lbItem = SendMessage(hList, LB_GETCURSEL, 0, 0);
		int id = SendMessage(hList, LB_GETITEMDATA, lbItem, 0);
		if (lbItem != LB_ERR)
		{
			WCHAR buf[SIZE];
			wsprintf(buf, TEXT("ID: %u \nThreads: %u \nPriority: %u \nName: %s"), myProcess[id].th32ProcessID, 
				myProcess[id].cntThreads, myProcess[id].pcPriClassBase,myProcess[id].szExeFile);
			MessageBox(hDialog, buf, TEXT("Info"), MB_OK | MB_ICONINFORMATION);

			UpdateSnapshot();
		}
	}
	else if (id == IDC_TERMINATE_PROC)
	{
		int lbItem = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
		int id = (int)SendMessage(hList, LB_GETITEMDATA, lbItem, 0);
		if (lbItem != LB_ERR)
		{
			HANDLE hProcess;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, myProcess[id].th32ProcessID);
			TerminateProcess(hProcess, 0);

			UpdateSnapshot();
		}
		else
		{
			MessageBox(hwnd, TEXT("Error"), TEXT("Error"), MB_OK | MB_ICONSTOP);
		}
	}
	else if (id == IDC_NEWPROC)
	{
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		WCHAR buf[SIZE];
		GetWindowText(hEdit, buf, length + 1);
		STARTUPINFO s = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION p;
		BOOL fs = CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &s, &p);
		if (fs)
		{
			CloseHandle(p.hThread);
			CloseHandle(p.hProcess);

			UpdateSnapshot();
		}
		else
		{
			MessageBox(hwnd, TEXT("Error"), TEXT("Error"), MB_OK | MB_ICONSTOP);
		}
	}
}

INT_PTR CALLBACK CSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}