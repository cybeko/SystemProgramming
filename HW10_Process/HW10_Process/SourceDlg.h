#pragma once
#include "header.h"

void OpenConsole();

class CSourceDlg
{
public:
	static CSourceDlg* ptr;
	static const int SIZE = 150;

	HANDLE hSnapShot;
	PROCESSENTRY32 pe32;
	vector<PROCESSENTRY32> myProcess;

	HWND hDialog;
	HWND hList;
	HWND hEdit;
	HWND hButRefresh, hButTerminate, hButInfo, hButCreate;

	void RefreshSnapShot();
	void UpdateSnapshot();

	CSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
};