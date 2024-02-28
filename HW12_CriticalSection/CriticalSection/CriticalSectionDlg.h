#pragma once
#include "header.h"

void OpenConsole();

class CriticalSectionDlg
{
public:
	CriticalSectionDlg(void);
public:
	~CriticalSectionDlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CriticalSectionDlg* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	//vector<string> fileNames;
	const int fileAmt = 3;

	string resFileName = "result.txt";
	string initFileName = "initial.txt";

};
